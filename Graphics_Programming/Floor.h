#pragma once
#include "SceneObject.h"
#include "Texture2D.h"

class Floor : public SceneObject {
private:
    Texture2D* _texture;
    float _size; // how big the ground is
    float _tiling; // how many times the pic repeats

public:
    Floor(Texture2D* texture, float size, float tiling);
    ~Floor();

    float GetTerrainHeight(float x, float z); // need this so we dont fall through the floor
    void Update() override;
    void Draw() override; // put the floor on the screen
};