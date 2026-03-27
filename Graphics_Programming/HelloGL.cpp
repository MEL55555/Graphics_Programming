#include "HelloGL.h"
#include <cstdlib>
// Camera + rotation controls
float camX = 0.0f;
float camY = 0.0f;
float camZ = -20.0f;
float rotX = 0.0f;
float rotY = 0.0f;
int lastMouseX = 400;
int lastMouseY = 300;
// ---------------- KEYBOARD ----------------
void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': camZ += 0.5f; break;
    case 's': camZ -= 0.5f; break;
    case 'a': camX += 0.5f; break;
    case 'd': camX -= 0.5f; break;
        // RESET CAMERA
    case 'r':
        camX = 0.0f;
        camY = 0.0f;
        camZ = -20.0f;
        rotX = 0.0f;
        rotY = 0.0f;
        break;
    }
}
// ---------------- MOUSE LOOK --------------
void MouseMotion(int x, int y)
{
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    rotY += dx * 0.2f;
    rotX += dy * 0.2f;
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}
// ------------------------------------------
HelloGL::~HelloGL() {}
HelloGL::HelloGL(int argc, char* argv[])
{
    GLUTCallbacks::Init(this);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OBJ Loader");
    glClearColor(0, 0, 0, 1);
    // DEPTH
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // CULLING
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // WIREFRAME MODE (visible without lighting)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // INPUT
    glutDisplayFunc(GLUTCallbacks::Display);
    glutKeyboardFunc(Keyboard);
    glutPassiveMotionFunc(MouseMotion);
    // TIMER
    glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
    // LOAD OBJ
    if (!Cube::Load((char*)"teapot.obj"))
    {
        MessageBoxA(NULL, "FAILED TO LOAD OBJ", "ERROR", MB_OK);
    }
    // CREATE OBJECTS
    for (int i = 0; i < CUBE_COUNT; i++)
    {
        float x = (rand() % 20 - 10);
        float y = (rand() % 20 - 10);
        float z = (rand() % 20 - 10);
        cubes[i] = new Cube(x, y, z);
    }
    glutMainLoop();
}
// ------------------------------------------
void HelloGL::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // ROTATION (mouse look)
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    
    glTranslatef(camX, camY, camZ);
    // DRAW OBJECTS
    for (int i = 0; i < CUBE_COUNT; i++)
        cubes[i]->Draw();
    glFlush();
}
// ------------------------------------------
void HelloGL::Update()
{
    for (int i = 0; i < CUBE_COUNT; i++)
        cubes[i]->Update();
    glutPostRedisplay();
}