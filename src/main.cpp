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
#include <GL/glew.h>
#include "windows.h"
#define GLM_FORCE_RADIANS // use radians
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SDL.h"
#undef main

//variables used to store window/gl states
SDL_Window* win;
SDL_GLContext context;
GLenum glew;
SDL_DisplayMode DM;
//simulation states
bool run = true;
bool rotate = false;
//variables used to handle color and location positions
float x, y, z;
float r = 0, g = 0, b = 0;
//dynamic sized arrays used to store point location and color data
std::vector<glm::vec3> points;
std::vector<glm::vec4> colors;
//constants used in algorithm later
 float sigma = 10;
 float beta = 8.0 / 3.0;
 float rho = 28;
 float dt = 0.02;

int init()
{
	bool ask = true; //if false skip
	std::cout << "Use defualts? Y/N ";
	std::string in;
	std::cin >> in; 
	std::transform(in.begin(), in.end(), in.begin(), ::toupper);//uppers user input
	try
	{
		if (in.find("Y") != std::string::npos) //find y
		{
			ask = false;
		}
	}
	catch (std::exception)
	{
		printf("Something happened, my apologies. I'll use the default values");
		system("pause");
		ask = false; //default values on error
	}

	if (ask) // use user variables
	{
		std::cout << "Enter a value for Sigma: ";
		std::string sig;
		std::cin >> sig;
		try {
			sigma = std::stof(sig);//convert user input to float
		}
		catch (std::exception ex)
		{
			printf("Can only accept float using default"); //on error use default value
			sigma = 10;
		}

		std::cout << "Enter a value for beta: ";
		std::string bet;
		std::cin >> bet;
		try {
			beta = std::stof(bet);
		}
		catch (std::exception ex)
		{
			printf("Can only accept float, using default");
			beta = 8.0 / 3.0;
		}

		std::cout << "Enter a value for rho: ";
		std::string rhos;
		std::cin >> rhos;
		try {
			rho = std::stof(rhos);
		}
		catch (std::exception ex)
		{
			printf("Can only accept float, using default");
			rho = 28.0;
		}

		std::cout << "Enter a value for deltatime: ";
		std::string dti;
		std::cin >> dti;
		try {
			dt = std::stof(dti);
		}
		catch (std::exception ex)
		{
			printf("Can only accept float, using default");
			dt = 0.02;
		}
	}


	//load video and events
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0)
	{
		//if errored print error and wait for user input
		printf("%s\n", SDL_GetError());
		system("pause");
		return 1;
	}

	//get display information
	SDL_GetDesktopDisplayMode(0, &DM);

	//create window, if didn't load successfully, print error and wait for user input
	win = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DM.w/2, DM.h/2, SDL_WINDOW_OPENGL);
	if (win == nullptr)
	{
		printf("%s\n", SDL_GetError());
		system("pause");
		return 1;
	}

	//initialize glew
	glew = glewInit();
	
	//create OpenGL context
	context = SDL_GL_CreateContext(win);

	//add starter point
	points.push_back({ 1.0, 1.0, 1.0 });

	//scale size for viewport
	float scalesize = 0.015;

	//scale by scalesize
	glScalef(scalesize,scalesize,scalesize);
	
	return 0;
}

//amount to increase color value by
float amount = 0.01;
glm::vec3 getCol()
{
	r += amount; //increment by amount
	if (r > 1) //if r is more than 1
	{
		r = 1; //hold r at 1
		g += amount;
		if (g > 1)
		{
			g = 0;
			b += amount;
			if (b > 1)
			{
				//if all are 1, reset
				r = 0;
				g = 0;
				b = 0;
			}
		}
	}
	return { r, g, b }; //return data
}

void process()
{
	SDL_Event evnt; //gets event
	if (SDL_PollEvent(&evnt)) //gets event information
	{
		if (evnt.type == SDL_QUIT) //if quit event
		{
			run = false; //close app
		}
		if (evnt.type == SDL_KEYDOWN) //if key pressed
		{
			if (evnt.key.keysym.sym == SDLK_SPACE) //if space key pressed
			{
				if (rotate) //toggle rotate boolean
					rotate = false;
				else
					rotate = true;
			}
		}
	}
}

void update()
{
	glm::vec3 point = points.back(); //gets last item in point array

	float dx = (sigma * (point.y - point.x)) * dt; //calculate delta x
	float dy = (point.x * (rho - point.z) - point.y) * dt; //calculates delta y
	float dz = (point.x * point.y - beta * point.z) * dt; //calculates delta z

	x = x + dx; //increment global x by x + delta x
	y = y + dy; //increment global y by y + delta y
	z = z + dz; //increment global z by z + delta z

	glm::vec3 col = getCol(); //get the next color value
	points.push_back(glm::vec3(x,y,z)); //push new point to dynamic array
	colors.push_back(glm::vec4(col, 1)); //push new color value to dynamic array

	printf("x: %f , y: %f, z: %f , r: %f , g: %f , b: %f", x, y, z, col.x, col.y, col.z); //print data ~~debug information~~
	printf("\n"); //new line
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear background
			
	glEnableClientState(GL_VERTEX_ARRAY); //enable vertex array
	glEnableClientState(GL_COLOR_ARRAY); //enable color array
	glVertexPointer(3, GL_FLOAT, 0, &points[0]); //feed vertex array data
	glColorPointer(4, GL_FLOAT, 0, &colors[0]); //feed color array data
	glDrawArrays(GL_LINE_LOOP, 0, points.capacity()); //draw everything in the vertex array
	glDisableClientState(GL_VERTEX_ARRAY); //disable vertex array
	glDisableClientState(GL_COLOR_ARRAY); //disable color array

	if(rotate) //if rotateable
		glRotatef(1, 0, 1, 0); //rotate

	SDL_GL_SwapWindow(win); //swap window with new data
}

int main()
{
	if (init() != 0) //initialize
		return 1; //if error, close

	while (run) //while able to run
	{
		process(); //process input
		update(); //update simulation
		draw(); //draw simulation
	}

	SDL_GL_DeleteContext(context); //upon close, delete OpenGL context

	SDL_Quit(); //quit SDL

	return 0; //clean close
}