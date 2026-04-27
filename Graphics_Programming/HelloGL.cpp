#include "HelloGL.h"
#include "Cube.h"
#include "MeshLoader.h"
#include "GLUTCallbacks.h" 
#include <cstdlib>
#include <cstdio> 
#include <cmath>  

float camX = 0.0f; float camY = 0.0f; float camZ = -20.0f;
float  rotX = 0.0f; float rotY = 0.0f;

const int WINDOW_WIDTH = 1920;
const int  WINDOW_HEIGHT = 1080;

HelloGL* currentInstance = nullptr;

void Keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 1.0f;
    float radY = rotY * (3.14159f / 180.0f);

    switch (key) {
    case 27:  exit(0); break;
    case 'w':
        camX -= sin(radY) * moveSpeed;
        camZ += cos(radY) * moveSpeed;
        break;
    case 's':
        camX += sin(radY) * moveSpeed;
        camZ -= cos(radY) * moveSpeed;
        break;
    case 'a':
        camX += cos(radY) * moveSpeed;
        camZ += sin(radY) * moveSpeed;
        break;
    case 'd':
        camX -= cos(radY) * moveSpeed;
        camZ -= sin(radY) * moveSpeed;
        break;
    case 'g': case 'G': if (currentInstance) currentInstance->TryPickupOrDrop(); break;
    case 'r':  camX = 0.0f; camY = 0.0f; camZ = -20.0f; rotX = 0.0f; rotY = 0.0f; break;
    }
}

void MouseMotion(int x, int y) {
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    if (x != centerX || y != centerY) {
        int dx = x - centerX;
        int dy = y - centerY;

        rotY += (float)dx * 0.15f;
        rotX += (float)dy * 0.15f;

        if (rotX > 90.0f) rotX = 90.0f;
        if (rotX < -90.0f) rotX = -90.0f;

        glutWarpPointer(centerX, centerY);
    }
}

HelloGL::HelloGL(int argc, char* argv[]) {
    currentInstance = this;
    _root = nullptr; myTexture = nullptr; skyTexture = nullptr; _score = 0;
    InitGL(argc, argv);
    InitObjects();
    glutMainLoop();
}

HelloGL::~HelloGL() {
    DeleteList(&_root);
    if (myTexture) delete myTexture;
    if (skyTexture)  delete skyTexture;
}

void HelloGL::TryPickupOrDrop() {
    ListNode* temp = _root;
    while (temp != nullptr) {
        if (temp->object->IsHeld()) {
            temp->object->SetIsHeld(false);
            return;
        }
        temp = temp->next;
    }

    temp = _root;
    SceneObject* bestTarget = nullptr;
    float bestDot = 0.85f;
    float maxReach = 30.0f;

    float radY = rotY * (3.14159f / 180.0f);
    float radX = rotX * (3.14159f / 180.0f);
    float lookX = sin(radY) * cos(radX);
    float lookY = -sin(radX);
    float lookZ = -cos(radY) * cos(radX);

    while (temp != nullptr) {
        Vector3 objPos = temp->object->GetPosition();
        float dirX = objPos.x - (-camX);
        float dirY = objPos.y - (-camY);
        float dirZ = objPos.z - (-camZ);
        float dist = (float)sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

        if (dist < maxReach) {
            dirX /= dist; dirY /= dist; dirZ /= dist;
            float dot = (lookX * dirX) + (lookY * dirY) + (lookZ * dirZ);
            if (dot > bestDot) {
                bestDot = dot;
                bestTarget = temp->object;
            }
        }
        temp = temp->next;
    }
    if (bestTarget) bestTarget->SetIsHeld(true);
}

void HelloGL::InitGL(int argc, char* argv[]) {
    GLUTCallbacks::Init(this);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int startX = (screenWidth - WINDOW_WIDTH) / 2;
    int startY = (screenHeight - WINDOW_HEIGHT) / 2;

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(startX, startY);
    glutCreateWindow("graphics programming");

    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    glutDisplayFunc(GLUTCallbacks::Display);
    glutKeyboardFunc(Keyboard);
    glutPassiveMotionFunc(MouseMotion);
    glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
}

void HelloGL::InitObjects() {
    Mesh* barrelMesh = MeshLoader::Load((char*)"barrel.obj");
    myTexture = new Texture2D();
    myTexture->Load((char*)"barrel.raw", 512, 512);
    skyTexture = new Texture2D();
    skyTexture->Load((char*)"sky.raw", 512, 512);

    for (int i = 0; i < 15; i++) {
        SceneObject* p = new Cube(barrelMesh, myTexture, (float)(rand() % 100 - 50), 0.0f, (float)(rand() % 100 - 50));
        AddObjectToList(p);
    }
}

void HelloGL::Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 3000.0f);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // draw skybox
    glPushMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glScalef(1500.0f, 1500.0f, 1500.0f);
    if (skyTexture) glBindTexture(GL_TEXTURE_2D, skyTexture->GetID());
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1); glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 1); glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1); glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, -1); glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1); glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, 1, 1); glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(1, 1); glVertex3f(-1, -1, -1); glTexCoord2f(0, 1); glVertex3f(1, -1, -1);
    glTexCoord2f(0, 0); glVertex3f(1, -1, 1); glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1); glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, 1); glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1); glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, 1); glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glEnd();
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glPopMatrix();

    // move camera
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glTranslatef(camX, camY, camZ);

    GLfloat light_pos[] = { 0, 100, 0, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    ListNode* temp = _root;
    while (temp != nullptr) {
        temp->object->Draw();
        temp = temp->next;
    }

    // drawing hud last so its on top 
    sprintf_s(_scoreText, 256, "barrels bumped: %d", _score);
    DrawText2D(_scoreText, 50, 1000); // moved it higher up on screen

    // crosshair in the middle
    DrawText2D("+", (WINDOW_WIDTH / 2) - 5, (WINDOW_HEIGHT / 2) - 5);

    glutSwapBuffers();
}

void HelloGL::Update() {
    ListNode* temp = _root;
    while (temp != nullptr) {
        if (temp->object->IsHeld()) {
            float radY = rotY * (3.14159f / 180.0f);
            float radX = rotX * (3.14159f / 180.0f);
            float  dist = 18.0f;
            float newX = -camX + (dist * sin(radY) * cos(radX));
            float newY = -camY - (dist * sin(radX));
            float newZ = -camZ - (dist * cos(radY) * cos(radX));
            temp->object->SetPosition(newX, newY, newZ);
        }
        else {
            // bump into barrels
            Vector3 objPos = temp->object->GetPosition();
            float dX = objPos.x - (-camX);
            float dY = objPos.y - (-camY);
            float dZ = objPos.z - (-camZ);
            float dist = sqrt(dX * dX + dY * dY + dZ * dZ);
            if (dist < 6.0f) {
                temp->object->SetPosition(objPos.x + dX, 0, objPos.z + dZ);
                _score++;
            }
        }
        temp->object->Update();
        temp = temp->next;
    }
    glutPostRedisplay();
}

void HelloGL::AddObjectToList(SceneObject* newObj) {
    ListNode* newNode = new ListNode;
    newNode->object = newObj;
    newNode->next = _root;
    _root = newNode;
}

void HelloGL::DeleteList(ListNode** node) {
    ListNode* pTemp = *node;
    while (pTemp != nullptr) {
        ListNode* next = pTemp->next;
        delete pTemp->object; delete pTemp;
        pTemp = next;
    }
    *node = nullptr;
}

void HelloGL::DrawText2D(const char* text, int x, int y) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST); // text wont get hidden by barrels
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glRasterPos2i(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}