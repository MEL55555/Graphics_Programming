#pragma once
#include <Windows.h> 
#include <gl/GL.h>

struct Vector3 { float x, y, z; };
struct Vector4 { float x, y, z, w; }; // Added for lighting colors

struct Vertex { float x, y, z; };
struct Color { float r, g, b; };
struct TexCoord { float u, v; };

struct Mesh
{
    Vertex* Vertices;
    Color* Colors;
    TexCoord* TexCoords;
    Vector3* Normals; // NEW: Added to store lighting direction
    GLushort* Indices;
    int VertexCount, ColorCount, TexCoordCount, NormalCount, IndexCount;
};