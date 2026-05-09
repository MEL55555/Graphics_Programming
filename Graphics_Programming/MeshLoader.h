#pragma once
#include "Structures.h"

class MeshLoader
{
public:
	// this gets the 3d models into the game
	static Mesh* Load(char* path);
};