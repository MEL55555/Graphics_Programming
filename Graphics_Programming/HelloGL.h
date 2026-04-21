#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>

#include "GLUTCallbacks.h"
#include "Structures.h"
#include "SceneObject.h"
#include "Texture2D.h"

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

    // NEW: Text rendering method
    void DrawText2D(const char* text, int x, int y);

private:
    SceneObject* objects[OBJECT_COUNT];
    Texture2D* myTexture;

    // NEW: Variables for our HUD
    int _score;
    char _scoreText[256];
};