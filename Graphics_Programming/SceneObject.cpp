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
    // clean up all the kids
    for (auto child : _children) delete child;
    _children.clear();
}

void SceneObject::Update()
{
    // make the thing spin around 
    _rotation += 1.5f;
    if (_rotation > 360.0f) _rotation -= 360.0f;

    for (auto child : _children)  child->Update();
}

void SceneObject::Draw()
{
    if (!_mesh || !_texture) return;

    glPushMatrix();
    glTranslatef(_position.x, _position.y, _position.z);
    glRotatef(_rotation, 0.0f, 1.0f, 0.0f);

    // use the picture
    glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // tell opengl where the points are
    glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
    glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

    glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    for (auto child : _children) child->Draw();
    glPopMatrix();
}