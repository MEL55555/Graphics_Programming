#include "StaticObject.h"

StaticObject::StaticObject(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture)
{
    _position = { x, y, z };
    _rotation = 0.0f;
}

StaticObject::~StaticObject() {}

void StaticObject::Update()
{
    for (auto child : _children) {
        child->Update();
    }
}

void StaticObject::Draw()
{
    if (_mesh == nullptr || _mesh->Vertices == nullptr) return;

    glPushMatrix();
    glTranslatef(_position.x, _position.y, _position.z);

    if (_texture != nullptr) glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // set up the pointers
    glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
    glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

    glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

    for (auto child : _children) {
        child->Draw();
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}