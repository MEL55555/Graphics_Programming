
#include "MeshLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

// helper to keep track of points
struct PackedVertex {
    int v, vt, vn;
    bool operator<(const PackedVertex that) const {
        return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
    };
};

Mesh* MeshLoader::Load(char* path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;

    // lists for the points and textures
    std::vector<Vertex> temp_vertices;
    std::vector<TexCoord> temp_uvs;
    std::vector<Vector3> temp_normals;

    std::vector<Vertex> out_vertices;
    std::vector<TexCoord> out_uvs;
    std::vector<Vector3> out_normals;
    std::vector<GLushort> out_indices;

    std::map<PackedVertex, GLushort> vertexCache;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            Vertex v; ss >> v.x >> v.y >> v.z;
            temp_vertices.push_back(v);
        }
        else if (type == "vt") {
            TexCoord tc; ss >> tc.u >> tc.v;
            temp_uvs.push_back(tc);
        }
        else if (type == "vn") {
            Vector3 vn; ss >> vn.x >> vn.y >> vn.z;
            temp_normals.push_back(vn);
        }
        else if (type == "f") {
            // go through the faces
            for (int i = 0; i < 3; i++) {
                std::string vertexStr;
                ss >> vertexStr;

                int vIdx = 0, vtIdx = 0, vnIdx = 0;

                size_t firstSlash = vertexStr.find('/');
                size_t lastSlash = vertexStr.find_last_of('/');

                vIdx = std::stoi(vertexStr.substr(0, firstSlash)) - 1;

                if (firstSlash != lastSlash && (lastSlash - firstSlash) > 1) {
                    vtIdx = std::stoi(vertexStr.substr(firstSlash + 1, lastSlash - firstSlash - 1)) - 1;
                }

                if (lastSlash != std::string::npos) {
                    vnIdx = std::stoi(vertexStr.substr(lastSlash + 1)) - 1;
                }

                PackedVertex packed = { vIdx, vtIdx, vnIdx };

                // check if we already have this point
                if (vertexCache.count(packed)) {
                    out_indices.push_back(vertexCache[packed]);
                }
                else {
                    GLushort newIndex = (GLushort)out_vertices.size();
                    out_vertices.push_back(temp_vertices[vIdx]);

                    if (vtIdx >= 0 && vtIdx < temp_uvs.size()) {
                        TexCoord tc = temp_uvs[vtIdx];
                        tc.v = 1.0f - tc.v;
                        out_uvs.push_back(tc);
                    }
                    else {
                        out_uvs.push_back({ 0, 0 });
                    }

                    if (vnIdx >= 0 && vnIdx < temp_normals.size()) {
                        out_normals.push_back(temp_normals[vnIdx]);
                    }
                    else {
                        out_normals.push_back({ 0, 1, 0 });
                    }

                    out_indices.push_back(newIndex);
                    vertexCache[packed] = newIndex;
                }
            }
        }
    }
    file.close();

    // build the final mesh
    Mesh* mesh = new Mesh();
    mesh->VertexCount = (int)out_vertices.size();
    mesh->IndexCount = (int)out_indices.size();

    mesh->Vertices = new Vertex[mesh->VertexCount];
    mesh->TexCoords = new TexCoord[mesh->VertexCount];
    mesh->Normals = new Vector3[mesh->VertexCount];
    mesh->Indices = new GLushort[mesh->IndexCount];
    mesh->Colors = new Color[mesh->VertexCount];

    for (int i = 0; i < mesh->VertexCount; i++) {
        mesh->Vertices[i] = out_vertices[i];
        mesh->TexCoords[i] = out_uvs[i];
        mesh->Normals[i] = out_normals[i];
        mesh->Colors[i] = { 1.0f, 1.0f, 1.0f };
    }

    for (int i = 0; i < mesh->IndexCount; i++) {
        mesh->Indices[i] = out_indices[i];
    }

    return mesh;
}
