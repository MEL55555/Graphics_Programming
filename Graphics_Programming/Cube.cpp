#include "Cube.h"

// ---------- STATIC ARRAY DATA ----------
Vertex Cube::_vertices[8] =
{
    {1,1,1},  {-1,1,1},
    {-1,-1,1},{1,-1,1},
    {1,-1,-1},{1,1,-1},
    {-1,1,-1},{-1,-1,-1}
};

Color Cube::_colors[8] =
{
    {1,1,1},{1,1,0},
    {1,0,0},{1,0,1},
    {0,0,1},{0,1,1},
    {0,1,0},{0,0,0}
};

GLushort Cube::_indices[36] =
{
    0,1,2, 2,3,0,
    0,3,4, 4,5,0,
    0,5,6, 6,1,0,
    1,6,7, 7,2,1,
    7,4,3, 3,2,7,
    4,7,6, 6,5,4
};

// ---------- CLASS IMPLEMENTATION ----------

Cube::Cube(float x, float y, float z)
{
    _rotation = 0.0f;
    _position = { x, y, z };
}

Cube::~Cube() {}

void Cube::Update()
{
    _rotation += 0.5f;
    if (_rotation >= 360)
        _rotation = 0;
}

void Cube::Draw()
{
    glPushMatrix();

    // POSITION FIRST (IMPORTANT)
    glTranslatef(_position.x, _position.y, _position.z);

    // ROTATION SECOND
    glRotatef(_rotation, 1, 1, 0);

    // DRAW USING ARRAYS
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, _vertices);
    glColorPointer(3, GL_FLOAT, 0, _colors);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, _indices);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}