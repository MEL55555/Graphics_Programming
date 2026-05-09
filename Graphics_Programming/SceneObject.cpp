#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture)
{
    _mesh = mesh;
    _texture = texture;
    _position = { 0.0f, 0.0f, 0.0f };
    _boundingRadius = 3.5f;
    _rotation = 0.0f;
    _isHeld = false;
}

SceneObject::~SceneObject()
{
    // kill all the kids before the parent dies
    for (auto child : _children)
    {
        delete child;
    }
    _children.clear();
}

void SceneObject::Update()
{
    // basic spinning logic
    _rotation += 1.5f;
    if (_rotation > 360.0f) _rotation -= 360.0f;

    // make sure everything attached to this also updates
    for (auto child : _children)
    {
        child->Update();
    }
}

void SceneObject::Draw()
{
    if (!_mesh || !_texture) return;

    // draw the main thing
    glPushMatrix();
    glTranslatef(_position.x, _position.y, _position.z);
    glRotatef(_rotation, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
    glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

    glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // draw the attached things relative to the parent
    for (auto child : _children)
    {
        child->Draw();
    }
    glPopMatrix();
}