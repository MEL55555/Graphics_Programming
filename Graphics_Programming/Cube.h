#pragma once
#include "SceneObject.h"
#include <Windows.h>
#include <gl/GL.h>

class Cube : public SceneObject
{
public:
    Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z);
    ~Cube();

    void Update() override;
    void Draw() override;

private:
    float _rotation;
    // _position has been REMOVED from here!
};