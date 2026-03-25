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

private:
    // Each cube has its own rotation + position
    float _rotation;
    Vector3 _position;

    // Static shared vertex/color/index arrays
    static Vertex _vertices[8];
    static Color _colors[8];
    static GLushort _indices[36];
};