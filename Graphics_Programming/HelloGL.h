#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>
#include "GLUTCallbacks.h"
#define REFRESHRATE 16
class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL(void);
	void Display();
	void Update();
	void DrawRectangle();
	void DrawSquare();
	void DrawTriangle();
private:
	float rotationRect;
	float rotationSquare;
	float rotationTriangle;
};