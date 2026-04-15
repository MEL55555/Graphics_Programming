#include "StaticObject.h"

StaticObject::StaticObject(Mesh* mesh, float x, float y, float z) : SceneObject(mesh)
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
        glPushMatrix();
        glTranslatef(_position.x, _position.y, _position.z);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
        glColorPointer(3, GL_FLOAT, 0, _mesh->Colors);

        glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
    }
}