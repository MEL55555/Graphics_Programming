#include "Floor.h"
#include <cmath>

Floor::Floor(Texture2D* texture, float size, float tiling) : SceneObject(nullptr, texture) {
    _texture = texture;
    _size = size;
    _tiling = tiling;
    _position = { 0.0f, -15.0f, 0.0f };
}

Floor::~Floor() {}


// ?? MUCH SOFTER TERRAIN (small hills, not mountains)
float Floor::GetTerrainHeight(float x, float z) {
    float y = sin(x * 0.03f) * cos(z * 0.03f) * 9.0f; // LOWER + SMOOTHER
    return y + _position.y;
}


// ?? NORMAL via neighbour sampling (super reliable)
Vector3 GetNormal(float x, float z, Floor* floor) {
    float offset = 1.0f;

    float hL = floor->GetTerrainHeight(x - offset, z);
    float hR = floor->GetTerrainHeight(x + offset, z);
    float hD = floor->GetTerrainHeight(x, z - offset);
    float hU = floor->GetTerrainHeight(x, z + offset);

    Vector3 normal;

    normal.x = hL - hR;
    normal.y = 2.0f; // strength of "up"
    normal.z = hD - hU;

    // normalize
    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    return normal;
}


void Floor::Update() {
    _isHeld = false;
}


void Floor::Draw() {
    if (!_texture) return;

    glShadeModel(GL_SMOOTH);

    GLfloat white[] = { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

    glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    glPushMatrix();

    // ?? Higher resolution = smoother terrain
    int gridRes = 150;
    float step = (_size * 2) / gridRes;

    for (int z = 0; z < gridRes; z++) {

        glBegin(GL_TRIANGLE_STRIP);

        for (int x = 0; x <= gridRes; x++) {

            float xPos = -_size + (x * step);

            float z1 = -_size + (z * step);
            float z2 = -_size + ((z + 1) * step);

            float y1 = GetTerrainHeight(xPos, z1);
            float y2 = GetTerrainHeight(xPos, z2);

            //  NORMAL 1
            Vector3 n1 = GetNormal(xPos, z1, this);
            glNormal3f(n1.x, n1.y, n1.z);
            glTexCoord2f((float)x / gridRes * _tiling, (float)z / gridRes * _tiling);
            glVertex3f(xPos, y1, z1);

            //  NORMAL 2
            Vector3 n2 = GetNormal(xPos, z2, this);
            glNormal3f(n2.x, n2.y, n2.z);
            glTexCoord2f((float)x / gridRes * _tiling, (float)(z + 1) / gridRes * _tiling);
            glVertex3f(xPos, y2, z2);
        }

        glEnd();
    }

    glPopMatrix();
}