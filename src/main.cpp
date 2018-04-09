#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <map>

//GLEW
#include <GL/glew.h>
#include "windows.h"

//OGL Maths
#define GLM_FORCE_RADIANS // use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL.h"

#undef main

SDL_Window* win;
SDL_GLContext context;
GLenum glew;
std::vector<glm::vec3> points;
std::vector<glm::vec4> colors;
//constants
const float sigma = 10;
const float beta = 8.0 / 3.0;
const float rho = 28;
const float dt = 0.02;
bool run = true;
SDL_DisplayMode DM;
float x, y, z;
float r = 0, g = 0, b = 0;
bool rotate = false;

int init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("%s\n", SDL_GetError());
		system("pause");
		return 1;
	}

	SDL_GetDesktopDisplayMode(0, &DM);

	win = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DM.w/2, DM.h/2, SDL_WINDOW_OPENGL);
	if (win == nullptr)
	{
		printf("%s\n", SDL_GetError());
		system("pause");
		return 1;
	}

	glew = glewInit();
	
	context = SDL_GL_CreateContext(win);

	points.push_back({ 1.0, 1.0, 1.0 });

	float scalesize = 0.015;

	glScalef(scalesize,scalesize,scalesize);

	//glPointSize(2.5);
	
	return 0;
}
float amount = 0.01;
glm::vec3 getCol()
{
	r += amount;
	if (r > 1)
	{
		r = 1;
		g += amount;
		if (g > 1)
		{
			g = 0;
			b += amount;
			if (b > 1)
			{
				r = 0;
				g = 0;
				b = 0;
			}
		}
	}
	return { r, g, b };
}

void process()
{
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt))
	{
		if (evnt.type == SDL_QUIT)
		{
			run = false;
		}
		if (evnt.type == SDL_KEYDOWN)
		{
			if (evnt.key.keysym.sym == SDLK_SPACE)
			{
				if (rotate)
					rotate = false;
				else
					rotate = true;
			}
		}
	}
}

void update()
{
	glm::vec3 point = points.back();

	float dx = (sigma * (point.y - point.x)) * dt;
	float dy = (point.x * (rho - point.z) - point.y) * dt;
	float dz = (point.x * point.y - beta * point.z) * dt;	

	x = x + dx;
	y = y + dy;
	z = z + dz;

	glm::vec3 col = getCol();
	points.push_back(glm::vec3(x,y,z));
	colors.push_back(glm::vec4(col, 1));

	printf("x: %f , y: %f, z: %f , r: %f , g: %f , b: %f", x, y, z, col.x, col.y, col.z);
	printf("\n");
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &points[0]);
	glColorPointer(4, GL_FLOAT, 0, &colors[0]);
	glDrawArrays(GL_LINE_LOOP, 0, points.capacity());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if(rotate)
		glRotatef(1, 0, 1, 0);

	SDL_GL_SwapWindow(win);
}

int main()
{
	if (init() != 0)
		return 1;

	while (run)
	{
		process();
		update();
		draw();
	}

	SDL_GL_DeleteContext(context);

	SDL_Quit();

	return 0;
}