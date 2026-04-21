#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture)
{
    _mesh = mesh;
    _texture = texture;

    // NEW: Initialize default collision values
    _position = { 0.0f, 0.0f, 0.0f };
    _boundingRadius = 3.5f; // Roughly the size of your scaled teapots
}

SceneObject::~SceneObject() {}
void SceneObject::Update() {}
void SceneObject::Draw() {}