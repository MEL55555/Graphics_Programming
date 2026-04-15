#include "HelloGL.h"
#include "Cube.h"
#include "StaticObject.h"
#include "MeshLoader.h"
#include <cstdlib>

float camX = 0.0f;
float camY = 0.0f;
float camZ = -20.0f;
float rotX = 0.0f;
float rotY = 0.0f;
int lastMouseX = 400;
int lastMouseY = 300;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': camZ += 0.5f; break;
    case 's': camZ -= 0.5f; break;
    case 'a': camX += 0.5f; break;
    case 'd': camX -= 0.5f; break;
    case 'r':
        camX = 0.0f; camY = 0.0f; camZ = -20.0f;
        rotX = 0.0f; rotY = 0.0f; break;
    }
}

void MouseMotion(int x, int y)
{
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    rotY += dx * 0.2f;
    rotX += dy * 0.2f;
    lastMouseX = x; lastMouseY = y;
    glutPostRedisplay();
}

HelloGL::~HelloGL() {}

HelloGL::HelloGL(int argc, char* argv[])
{
    InitGL(argc, argv);
    InitObjects();
    glutMainLoop();
}

void HelloGL::InitGL(int argc, char* argv[])
{
    GLUTCallbacks::Init(this);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OBJ Loader - Polymorphism");
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glutDisplayFunc(GLUTCallbacks::Display);
    glutKeyboardFunc(Keyboard);
    glutPassiveMotionFunc(MouseMotion);
    glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
}

void HelloGL::InitObjects()
{
    Mesh* myMesh = MeshLoader::Load((char*)"teapot.obj");

    if (myMesh == nullptr)
    {
        MessageBoxA(NULL, "FAILED TO LOAD OBJ", "ERROR", MB_OK);
        return;
    }

    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        float x = (rand() % 40 - 20);
        float y = (rand() % 40 - 20);
        float z = (rand() % 40 - 20);

        if (i < 15) {
            objects[i] = new Cube(myMesh, x, y, z);
        }
        else {
            objects[i] = new StaticObject(myMesh, x, y, z);
        }
    }
}

void HelloGL::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glTranslatef(camX, camY, camZ);

    for (int i = 0; i < OBJECT_COUNT; i++)
        objects[i]->Draw();

    glFlush();
}

void HelloGL::Update()
{
    for (int i = 0; i < OBJECT_COUNT; i++)
        objects[i]->Update();

    glutPostRedisplay();
}