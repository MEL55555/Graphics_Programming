#pragma once
#include <Windows.h> 
#include <gl/GL.h>

struct Vector3
{
    float x, y, z;
};

struct Vertex
{
    float x, y, z;
};

struct Color
{
    float r, g, b;
};

struct Mesh
{
    Vertex* Vertices;
    Color* Colors;
    GLushort* Indices;
    int VertexCount, ColorCount, IndexCount;
};