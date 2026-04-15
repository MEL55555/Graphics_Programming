#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>

#include "GLUTCallbacks.h"
#include "Structures.h"
#include "SceneObject.h"

#define REFRESHRATE 16
#define OBJECT_COUNT 25 

class HelloGL
{
public:
    HelloGL(int argc, char* argv[]);
    ~HelloGL();

    void Display();
    void Update();

    void InitObjects();
    void InitGL(int argc, char* argv[]);

private:
    SceneObject* objects[OBJECT_COUNT];
};