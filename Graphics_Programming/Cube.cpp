#include "Cube.h"
#include <cstdlib> 

Cube::Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture)
{
    _rotation = 0.0f;
    _position = { x, y, z };
    _boundingRadius = 5.0f;

    // make it spin at random speeds
    _rotationSpeed = (float)(rand() % 100) / 200.0f + 0.1f;
    _rotationAxis = { (float)(rand() % 10) / 10.0f, (float)(rand() % 10) / 10.0f, (float)(rand() % 10) / 10.0f };
}

Cube::~Cube() {}

void Cube::Update()
{
    // keep it turning
    _rotation += _rotationSpeed;
    if (_rotation >= 360.0f) _rotation = 0.0f;

    for (auto child : _children) child->Update();
}

void Cube::Draw()
{
    if (_mesh == nullptr || _mesh->Vertices == nullptr) return;

    glPushMatrix();
    glTranslatef(_position.x, _position.y, _position.z);
    glRotatef(_rotation, _rotationAxis.x, _rotationAxis.y, _rotationAxis.z);

    if (_texture != nullptr)  glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // send data to gpu
    glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
    glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

    glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

    for (auto child : _children) child->Draw();

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}