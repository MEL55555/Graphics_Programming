#include "HelloGL.h"
#include "Cube.h"
#include "StaticObject.h"
#include "MeshLoader.h"
#include <cstdlib>
#include <cstdio> // For formatting strings (sprintf_s)
#include <cmath>  // For collision math (std::sqrt, std::pow)

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

HelloGL::~HelloGL()
{
    delete myTexture;
}

HelloGL::HelloGL(int argc, char* argv[])
{
    _score = 0; // Initialize score
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
    glutCreateWindow("OBJ Loader");
    glClearColor(0, 0, 0, 1);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 0.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glutDisplayFunc(GLUTCallbacks::Display);
    glutKeyboardFunc(Keyboard);
    glutPassiveMotionFunc(MouseMotion);
    glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
}

void HelloGL::InitObjects()
{
    Mesh* myMesh = MeshLoader::Load((char*)"teapot.obj");

    myTexture = new Texture2D();
    myTexture->Load((char*)"penguins.raw", 512, 512);

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
            objects[i] = new Cube(myMesh, myTexture, x, y, z);
        }
        else {
            objects[i] = new StaticObject(myMesh, myTexture, x, y, z);
        }
    }
}

// ==========================================
// Function to draw 2D Text over the 3D scene
// ==========================================
void HelloGL::DrawText2D(const char* text, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Create a 2D canvas matching window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Temporarily turn off 3D effects so text draws flat on the screen
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 0.0f); // Make text Yellow
    glRasterPos2i(x, y);

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    // Turn 3D effects back on
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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

    // ==========================================
    // Draw the HUD Text
    // ==========================================
    sprintf_s(_scoreText, "Bumps: %d | Move with W/A/S/D to crash into objects!", _score);
    DrawText2D(_scoreText, 10, 570); // Draw at the top left corner

    glFlush();
}

void HelloGL::Update()
{
    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        objects[i]->Update();

        // ==========================================
        // Basic Bounding Sphere Collision!
        // ==========================================
        Vector3 objPos = objects[i]->GetPosition();
        float objRadius = objects[i]->GetBoundingRadius();

        // The camera (player) position is inverted in OpenGL math
        float pX = -camX;
        float pY = -camY;
        float pZ = -camZ;

        // Calculate the 3D distance between Camera and Object
        float distance = std::sqrt(
            std::pow(pX - objPos.x, 2) +
            std::pow(pY - objPos.y, 2) +
            std::pow(pZ - objPos.z, 2)
        );

        // If the distance is smaller than the size of the object, we crashed!
        if (distance < objRadius)
        {
            _score += 1; // Increase bump score

            // "Bounce" the player backward so they don't get stuck inside the object
            camZ -= 2.0f;
        }
    }

    glutPostRedisplay();
}