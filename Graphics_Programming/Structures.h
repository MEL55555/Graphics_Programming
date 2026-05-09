#pragma once
#include <Windows.h> 
#include <gl/GL.h>
#include <cmath>

// basic math stuff
struct Vector3 { float x, y, z; };
struct Vector4 { float x, y, z, w; };

// dots and colors
struct Vertex { float x, y, z; };
struct Color { float r, g, b; };
struct TexCoord { float u, v; };

class SceneObject;

// making a list for all the stuff
struct ListNode {
    SceneObject* object;
    ListNode* next;
};

struct Mesh {
    Vertex* Vertices;
    Color* Colors;
    TexCoord* TexCoords;
    Vector3* Normals;
    GLushort* Indices;
    int VertexCount, ColorCount, TexCoordCount, NormalCount, IndexCount;
};