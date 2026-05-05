#include "HelloGL.h"
#include "Cube.h"
#include "MeshLoader.h"
#include "GLUTCallbacks.h" 
#include <cstdlib>
#include <cstdio> 
#include <cmath>  
#include <iostream>

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

//[cite: 2] Initializing the Linked List head as nullptr
HelloGL::HelloGL(int argc, char* argv[]) {
    currentInstance = this;
    _root = nullptr;
    myTexture = nullptr;
    skyTexture = nullptr;
    grassTexture = nullptr;
    _score = 0;
    InitGL(argc, argv);
    InitObjects();
    glutMainLoop();
}

//[cite: 2] Using the DeleteList tutorial logic for cleanup
HelloGL::~HelloGL() {
    DeleteList(&_root);
    if (myTexture) delete myTexture;
    if (skyTexture) delete skyTexture;
    if (grassTexture) delete grassTexture;
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

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Graphics Programming - Linked Lists & Smooth Terrain");

    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    // STATE FIXES: Ensure normals and shading are set for the Floor tutorial
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

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

    grassTexture = new Texture2D();
    grassTexture->Load((char*)"grass.raw", 512, 512);

    _levelFloor = new Floor(grassTexture, 1000.0f, 60.0f);
    AddObjectToList(_levelFloor);

    for (int i = 0; i < 15; i++) {
        SceneObject* p = new Cube(barrelMesh, myTexture, (float)(rand() % 200 - 100), 0.0f, (float)(rand() % 200 - 100));
        AddObjectToList(p);
    }
}

void HelloGL::Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 3000.0f);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // Skybox Rendering
    glPushMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glRotatef(rotX, 0.0f, 0.0f, 0.0f);
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

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glTranslatef(camX, camY, camZ);

    GLfloat light_pos[] = { 0, 100, 0, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    //[cite: 2] Iterating through the linked list for Draw
    ListNode* temp = _root;
    while (temp != nullptr) {
        temp->object->Draw();
        temp = temp->next;
    }

    sprintf_s(_scoreText, 256, "barrels bumped: %d", _score);
    DrawText2D(_scoreText, 50, 1000);
    DrawText2D("+", (WINDOW_WIDTH / 2) - 5, (WINDOW_HEIGHT / 2) - 5);

    glutSwapBuffers();
}

void HelloGL::Update() {
    //[cite: 2] Iterating through the linked list for Update
    ListNode* temp = _root;

    while (temp != nullptr) {
        SceneObject* obj = temp->object;
        Vector3 objPos = obj->GetPosition();
        float radius = obj->GetBoundingRadius();

        if (obj->IsHeld()) {
            float radY = rotY * (3.14159f / 180.0f);
            float radX = rotX * (3.14159f / 180.0f);
            float dist = 18.0f;

            float newX = -camX + (dist * sin(radY) * cos(radX));
            float newY = -camY - (dist * sin(radX));
            float newZ = -camZ - (dist * cos(radY) * cos(radX));

            float floorH = _levelFloor->GetTerrainHeight(newX, newZ);
            if (newY < floorH + radius) newY = floorH + radius;

            obj->SetPosition(newX, newY, newZ);
        }
        else if (obj != (SceneObject*)_levelFloor) {
            float floorH = _levelFloor->GetTerrainHeight(objPos.x, objPos.z);
            if (objPos.y < floorH + radius) {
                obj->SetPosition(objPos.x, floorH + radius, objPos.z);
                objPos.y = floorH + radius;
            }

            // Object-to-Object Collisions using the List
            ListNode* otherNode = _root;
            while (otherNode != nullptr) {
                SceneObject* other = otherNode->object;

                if (other != obj && other != (SceneObject*)_levelFloor) {
                    Vector3 otherPos = other->GetPosition();
                    float otherRadius = other->GetBoundingRadius();

                    float dx = objPos.x - otherPos.x;
                    float dz = objPos.z - otherPos.z;
                    float distSq = dx * dx + dz * dz;
                    float minDist = radius + otherRadius;

                    if (distSq < minDist * minDist && distSq > 0.0001f) {
                        float dist = sqrt(distSq);
                        float overlap = (minDist - dist) * 0.5f;

                        dx /= dist;
                        dz /= dist;

                        obj->SetPosition(objPos.x + dx * overlap, objPos.y, objPos.z + dz * overlap);
                        other->SetPosition(otherPos.x - dx * overlap, otherPos.y, otherPos.z - dz * overlap);
                    }
                }
                otherNode = otherNode->next;
            }

            // Camera bump
            float camDistX = objPos.x - (-camX);
            float camDistZ = objPos.z - (-camZ);
            float camDist = sqrt(camDistX * camDistX + camDistZ * camDistZ);

            if (camDist < 6.0f) {
                obj->SetPosition(objPos.x + (camDistX / camDist) * 2.0f, objPos.y, objPos.z + (camDistZ / camDist) * 2.0f);
                _score++;
            }
        }

        obj->Update();
        temp = temp->next;
    }

    glutPostRedisplay();
}

//[cite: 2] Tutorial 13 Task 1: InsertFirst logic
void HelloGL::AddObjectToList(SceneObject* newObj) {
    ListNode* newNode = new ListNode;
    newNode->object = newObj;
    newNode->next = _root;
    _root = newNode;
}

//[cite: 2] Tutorial 13 Task 1: Recursive-style iterative cleanup
void HelloGL::DeleteList(ListNode** node) {
    ListNode* pTemp = *node;
    while (pTemp != nullptr) {
        ListNode* next = pTemp->next;
        delete pTemp->object;
        delete pTemp;
        pTemp = next;
    }
    *node = nullptr;
}

void HelloGL::DrawText2D(const char* text, int x, int y) {
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_TEXTURE_2D);
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
    glEnable(GL_TEXTURE_2D); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}