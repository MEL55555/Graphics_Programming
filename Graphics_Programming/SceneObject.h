#pragma once
#include "Structures.h"
#include "Texture2D.h"
#include <vector> 

class SceneObject
{
protected:
    Mesh* _mesh;
    Texture2D* _texture;
    Vector3 _position; // where the thing is at
    float _boundingRadius;
    float _rotation;
    bool _isHeld = false; // check if we are holding it

    // list of all the tiny things attached to this object
    std::vector<SceneObject*> _children;

public:
    SceneObject(Mesh* mesh, Texture2D* texture);
    // clean up the mess when we are done
    virtual ~SceneObject();

    virtual void Update();
    virtual void Draw();

    // stuff to handle the object and its kids
    void AddChild(SceneObject* child) { _children.push_back(child); }
    void SetPosition(float x, float y, float z) { _position = { x, y, z }; }
    void SetIsHeld(bool held) { _isHeld = held; }
    bool IsHeld() const { return _isHeld; }

    Vector3 GetPosition() const { return _position; }
    float GetBoundingRadius() const { return _boundingRadius; }
};