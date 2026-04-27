#pragma once
#include "Structures.h"

class MeshLoader
{
public:
    // help load the 3d files
    static Mesh* Load(char* path);
};