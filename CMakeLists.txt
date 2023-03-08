cmake_minimum_required(VERSION 3.22)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib/x64)

add_executable(Lab4 main.cpp Vector3d.h Vector4d.h Matrix4d.h Triangle.h Math.h Light.h Shader.h obj/OBJ_Loader.h)

target_link_libraries(Lab4 libopengl32.a libfreeglut.a glu32.dll)