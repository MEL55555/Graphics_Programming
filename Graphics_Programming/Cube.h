#pragma once
#include "Structures.h"
#include <Windows.h>
#include <gl/GL.h>
class Cube
{
public:
	Cube(float x, float y, float z);
	~Cube();
	void Update();
	void Draw();
	static bool Load(char* path);
private:
	float _rotation;
	Vector3 _position;
	static Vertex* _vertices;
	static Color* _colors;
	static GLushort* _indices;
	static int _vertexCount;
	static int _colorCount;
	static int _indexCount;
};