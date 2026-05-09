#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/freeglut.h>
#include "Structures.h"
#include "SceneObject.h"
#include "Texture2D.h"
#include "Floor.h"

#define REFRESHRATE 16

class HelloGL {
public:
    HelloGL(int argc, char* argv[]);
    ~HelloGL();

    void Display();
    void Update();
    void InitObjects(); // making all the stuff
    void InitGL(int argc, char* argv[]);
    void DrawText2D(const char* text, int x, int y); // write words on the screen

    void TryPickupOrDrop();

    // functions to mess with the list of stuff
    void AddObjectToList(SceneObject* newObj);
    void DeleteList(ListNode** node);

private:
    ListNode* _root; // the start of the big list of things
    Texture2D* myTexture;
    Texture2D* skyTexture;
    Texture2D* grassTexture;
    Floor* _levelFloor;
    int _score; // the points player has
    char _scoreText[256];
};