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
    std::vector<TexCoord> tempTexCoords;
    std::vector<Vector3> tempNormals; // NEW
    std::vector<GLushort> tempIndices;
    std::string line;

    std::vector<int> vertexToTexCoordMap;
    std::vector<int> vertexToNormalMap;   // NEW

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            tempVertices.push_back(v);
            vertexToTexCoordMap.push_back(-1);
            vertexToNormalMap.push_back(-1); // NEW
        }
        else if (type == "vt")
        {
            TexCoord tc;
            ss >> tc.u >> tc.v;
            tempTexCoords.push_back(tc);
        }
        else if (type == "vn") // NEW: Parse Normals
        {
            Vector3 vn;
            ss >> vn.x >> vn.y >> vn.z;
            tempNormals.push_back(vn);
        }
        else if (type == "f")
        {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;

            auto processFaceVertex = [&](std::string s) {
                std::stringstream ss(s);
                std::string vIndexStr, vtIndexStr, vnIndexStr;

                std::getline(ss, vIndexStr, '/');
                int vIdx = std::stoi(vIndexStr) - 1;

                if (std::getline(ss, vtIndexStr, '/') && !vtIndexStr.empty()) {
                    int vtIdx = std::stoi(vtIndexStr) - 1;
                    vertexToTexCoordMap[vIdx] = vtIdx;
                }

                if (std::getline(ss, vnIndexStr, '/') && !vnIndexStr.empty()) {
                    int vnIdx = std::stoi(vnIndexStr) - 1;
                    vertexToNormalMap[vIdx] = vnIdx; // Map normal to vertex
                }
                return vIdx;
                };

            tempIndices.push_back(processFaceVertex(v1));
            tempIndices.push_back(processFaceVertex(v2));
            tempIndices.push_back(processFaceVertex(v3));
        }
    }
    file.close();

    Mesh* mesh = new Mesh();
    mesh->VertexCount = tempVertices.size();
    mesh->IndexCount = tempIndices.size();
    mesh->ColorCount = mesh->VertexCount;
    mesh->TexCoordCount = mesh->VertexCount;
    mesh->NormalCount = mesh->VertexCount; // NEW

    mesh->Vertices = new Vertex[mesh->VertexCount];
    mesh->Indices = new GLushort[mesh->IndexCount];
    mesh->Colors = new Color[mesh->ColorCount];
    mesh->TexCoords = new TexCoord[mesh->TexCoordCount];
    mesh->Normals = new Vector3[mesh->NormalCount]; // NEW

    for (int i = 0; i < mesh->VertexCount; i++)
    {
        mesh->Vertices[i] = tempVertices[i];
        mesh->Colors[i] = { 1.0f, 1.0f, 1.0f };

        // HACK: Planar mapping to project texture based on X/Y position
        mesh->TexCoords[i].u = mesh->Vertices[i].x * 0.2f;
        mesh->TexCoords[i].v = mesh->Vertices[i].y * 0.2f;

        // Apply mapped normals, default to pointing UP if missing
        if (vertexToNormalMap[i] != -1 && vertexToNormalMap[i] < tempNormals.size()) {
            mesh->Normals[i] = tempNormals[vertexToNormalMap[i]];
        }
        else {
            mesh->Normals[i] = { 0.0f, 1.0f, 0.0f };
        }
    }

    for (int i = 0; i < mesh->IndexCount; i++)
    {
        mesh->Indices[i] = tempIndices[i];
    }

    return mesh;
}