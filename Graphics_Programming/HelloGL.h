#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>
#include "Structures.h"
#include "SceneObject.h"
#include "Texture2D.h"

#define REFRESHRATE 16

class HelloGL
{
public:
    HelloGL(int argc, char* argv[]);
    ~HelloGL();

    void Display();
    void Update();
    void InitObjects();
    void InitGL(int argc, char* argv[]);
    void DrawText2D(const char* text, int x, int y);

    // logic for grabbing and dropping
    void TryPickupOrDrop();
    void AddObjectToList(SceneObject* newObj);
    void DeleteList(ListNode** node);

private:
    ListNode* _root;   // start of the list
    Texture2D* myTexture;
    Texture2D* skyTexture;
    int _score;
    char _scoreText[256];
};