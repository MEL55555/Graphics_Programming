#include "GLUTCallbacks.h"
#include "HelloGL.h"
namespace
{
    HelloGL* helloGL = nullptr;
}
namespace GLUTCallbacks
{
    void Init(HelloGL* gl)
    {
        helloGL = gl;
    }
    void Display()
    {
        if (helloGL != nullptr)
        {
            helloGL->Display();
        }
    }
}