#pragma once
#include "SceneObject.h"
#include "Texture2D.h"

class Floor : public SceneObject {
private:
    Texture2D* _texture;
    float _size;
    float _tiling;

public:
    Floor(Texture2D* texture, float size, float tiling);
    ~Floor();

    float GetTerrainHeight(float x, float z); // Required for physics
    void Update() override;
    void Draw() override;
};