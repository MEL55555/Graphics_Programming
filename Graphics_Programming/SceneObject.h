#pragma once
#include "Structures.h"
#include "Texture2D.h"

class SceneObject
{
protected:
    Mesh* _mesh;
    Texture2D* _texture;

    // NEW: Moved here so ALL objects share a position and size for collision
    Vector3 _position;
    float _boundingRadius;

public:
    SceneObject(Mesh* mesh, Texture2D* texture);
    virtual ~SceneObject();

    virtual void Update();
    virtual void Draw();

    // NEW: Getters for collision detection
    Vector3 GetPosition() const { return _position; }
    float GetBoundingRadius() const { return _boundingRadius; }
};