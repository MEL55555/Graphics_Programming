#pragma once
#include "SceneObject.h"
#include <Windows.h>
#include <gl/GL.h>

class StaticObject : public SceneObject
{
public:
    StaticObject(Mesh* mesh, Texture2D* texture, float x, float y, float z);
    ~StaticObject();

    void Update() override;
    void  Draw() override;
    // for the stuff that just sits there and doesnt move
};