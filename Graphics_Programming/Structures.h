#pragma once
#include <Windows.h> 
#include <gl/GL.h>

// basic math stuff
struct Vector3 { float x, y, z; };
struct  Vector4 { float x, y, z, w; };

// points and colors
struct Vertex { float x, y, z; };
struct Color { float r, g, b; };
struct TexCoord { float u, v; };


class  SceneObject;


struct  ListNode
{
    SceneObject* object;
    ListNode* next;
};

// data for the shapes
struct Mesh
{
    Vertex* Vertices;
    Color* Colors;
    TexCoord* TexCoords;
    Vector3* Normals;
    GLushort* Indices;
    int VertexCount, ColorCount, TexCoordCount, NormalCount, IndexCount;
};