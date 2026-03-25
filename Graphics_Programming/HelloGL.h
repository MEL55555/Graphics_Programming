#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>

#include "GLUTCallbacks.h"
#include "Cube.h"
#include "Structures.h"

#define REFRESHRATE 16
#define CUBE_COUNT 25

class HelloGL
{
public:
    HelloGL(int argc, char* argv[]);
    ~HelloGL();

    void Display();
    void Update();

private:
    Cube* cubes[CUBE_COUNT];
};