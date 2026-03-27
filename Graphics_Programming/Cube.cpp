#include "Cube.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// ---------- STATIC DATA ----------
Vertex* Cube::_vertices = nullptr;
Color* Cube::_colors = nullptr;
GLushort* Cube::_indices = nullptr;
int Cube::_vertexCount = 0;
int Cube::_colorCount = 0;
int Cube::_indexCount = 0;
// ---------- CONSTRUCTOR ----------
Cube::Cube(float x, float y, float z)
{
    _rotation = 0.0f;
    _position = { x, y, z };
}
Cube::~Cube() {}
// ---------- UPDATE ----------
void Cube::Update()
{
    _rotation += 0.2f;
    if (_rotation >= 360)
        _rotation = 0;
}
// ---------- DRAW ----------
void Cube::Draw()
{
    if (_vertices != nullptr && _indices != nullptr)
    {
        glPushMatrix();
        glTranslatef(_position.x, _position.y, _position.z);
        glRotatef(_rotation, 1, 1, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, _vertices);
        glColorPointer(3, GL_FLOAT, 0, _colors);
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, _indices);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
}
// ---------- OBJ LOADER ----------
bool Cube::Load(char* path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;
    std::vector<Vertex> tempVertices;
    std::vector<GLushort> tempIndices;
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        // VERTICES
        if (type == "v")
        {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            tempVertices.push_back(v);
        }
        // FACES (handles 1/1/1 format)
        else if (type == "f")
        {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            auto getIndex = [](std::string s)
                {
                    std::stringstream ss(s);
                    std::string index;
                    std::getline(ss, index, '/');
                    return std::stoi(index) - 1; // OBJ starts at 1
                };
            tempIndices.push_back(getIndex(v1));
            tempIndices.push_back(getIndex(v2));
            tempIndices.push_back(getIndex(v3));
        }
    }
    file.close();
    _vertexCount = tempVertices.size();
    _indexCount = tempIndices.size();
    _colorCount = _vertexCount;
    _vertices = new Vertex[_vertexCount];
    _indices = new GLushort[_indexCount];
    _colors = new Color[_colorCount];
    for (int i = 0; i < _vertexCount; i++)
    {
        _vertices[i] = tempVertices[i];
        // Give color (light grey so it's visible)
        _colors[i] = { 0.8f, 0.8f, 0.8f };
    }
    for (int i = 0; i < _indexCount; i++)
    {
        _indices[i] = tempIndices[i];
    }
    return true;
}