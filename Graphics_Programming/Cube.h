#pragma once
#include "SceneObject.h"

class Cube : public SceneObject
{
private:
    float _rotationSpeed;  // how fast it spins
    Vector3 _rotationAxis; // the way it turns 

public:
    Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z);
    ~Cube();

    void Update() override;
    void Draw() override;
};