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

	
	void DrawIndexedCubeAlt();

	
	void DrawPyramid();

	
	void DrawCube();
	void DrawCubeArray();

	struct Vertex
	{
		GLfloat x, y, z;
	};

	struct Color
	{
		GLfloat r, g, b;
	};

	static Vertex indexedVertices[];
	static Color indexedColors[];
	static GLushort indices[];
};