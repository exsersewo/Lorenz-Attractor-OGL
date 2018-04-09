#Set the project name
project(LorenzAttractor)

#Set the CMake version
cmake_minimum_required(VERSION 3.0)

#Use Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#Import all source files
file(GLOB src_files "src/*.c" "src/*.cpp" )

#Make a project, link in libraries from conan
ADD_EXECUTABLE(LorenzAttractor ${src_files})
TARGET_LINK_LIBRARIES(LorenzAttractor ${CONAN_LIBS})

#Set the C++ version
set (CMAKE_CXX_STANDARD 11)

#Set the project for VS to use.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "LorenzAttractor")

#Perform prebuild
add_custom_command(TARGET LorenzAttractor  PRE_BUILD COMMAND "pre_build_Script.bat")