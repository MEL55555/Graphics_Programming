#include "StaticObject.h"

StaticObject::StaticObject(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture)
{
    _position = { x, y, z };
}

StaticObject::~StaticObject() {}

void StaticObject::Update()
{
    // Deliberately empty! This object does not move or rotate.
}

void StaticObject::Draw()
{
    if (_mesh->Vertices != nullptr && _mesh->Indices != nullptr)
    {
        if (_texture != nullptr) {
            glBindTexture(GL_TEXTURE_2D, _texture->GetID());
        }

        glPushMatrix();
        glTranslatef(_position.x, _position.y, _position.z);

        // Define how the light interacts with this specific object's surface
        GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY); // NEW

        glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
        glColorPointer(3, GL_FLOAT, 0, _mesh->Colors);
        glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
        glNormalPointer(GL_FLOAT, 0, _mesh->Normals); // NEW

        glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

        glDisableClientState(GL_NORMAL_ARRAY); // NEW
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
    }
}