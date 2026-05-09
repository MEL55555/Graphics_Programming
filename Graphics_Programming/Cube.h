#pragma once
#include "SceneObject.h"

class Cube : public SceneObject
{
private:
    float _rotationSpeed; // how fast this thing spins
    Vector3 _rotationAxis;   // which way it turns 

public:
    Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z);
    ~Cube();

    void Update() override;
    void Draw() override; // draw the box
};