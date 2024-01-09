cmake_minimum_required(VERSION 3.22)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)

# seach
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
# include
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(Lab4 main.cpp Vector3d.h Vector4d.h Matrix4d.h Triangle.h Math.h Light.h Shader.h obj/OBJ_Loader.h)
