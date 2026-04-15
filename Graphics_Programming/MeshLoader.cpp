#include "MeshLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Mesh* MeshLoader::Load(char* path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;

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
        // FACES
        else if (type == "f")
        {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;

            auto getIndex = [](std::string s)
                {
                    std::stringstream ss(s);
                    std::string index;
                    std::getline(ss, index, '/');
                    return std::stoi(index) - 1;
                };

            tempIndices.push_back(getIndex(v1));
            tempIndices.push_back(getIndex(v2));
            tempIndices.push_back(getIndex(v3));
        }
    }
    file.close();

    Mesh* mesh = new Mesh();
    mesh->VertexCount = tempVertices.size();
    mesh->IndexCount = tempIndices.size();
    mesh->ColorCount = mesh->VertexCount;

    mesh->Vertices = new Vertex[mesh->VertexCount];
    mesh->Indices = new GLushort[mesh->IndexCount];
    mesh->Colors = new Color[mesh->ColorCount];

    for (int i = 0; i < mesh->VertexCount; i++)
    {
        mesh->Vertices[i] = tempVertices[i];
        mesh->Colors[i] = { 0.8f, 0.8f, 0.8f };
    }
    for (int i = 0; i < mesh->IndexCount; i++)
    {
        mesh->Indices[i] = tempIndices[i];
    }

    return mesh;
}