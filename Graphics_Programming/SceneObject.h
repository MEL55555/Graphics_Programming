#pragma once
#include "Structures.h"
#include "Texture2D.h"
#include <vector> 

class SceneObject
{
protected:
    Mesh* _mesh;
    Texture2D* _texture;
    Vector3 _position;
    float _boundingRadius;
    float _rotation;
    bool _isHeld = false;

    // Tutorial 14: This vector represents the child nodes in our Scene Graph tree
    std::vector<SceneObject*> _children;

public:
    SceneObject(Mesh* mesh, Texture2D* texture);
    // Tutorial 14: Virtual destructor is critical for recursive cleanup
    virtual ~SceneObject();

    virtual void Update();
    virtual void Draw();

    //[cite: 1, 2] Functions to build the hierarchy and manage state
    void AddChild(SceneObject* child) { _children.push_back(child); }
    void SetPosition(float x, float y, float z) { _position = { x, y, z }; }
    void SetIsHeld(bool held) { _isHeld = held; }
    bool IsHeld() const { return _isHeld; }

    Vector3 GetPosition() const { return _position; }
    float GetBoundingRadius() const { return _boundingRadius; }
};