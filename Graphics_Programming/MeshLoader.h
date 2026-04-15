#pragma once
#include "Structures.h"

class MeshLoader
{
public:
    static Mesh* Load(char* path);
};