
#include "GLUTCallbacks.h"
#include "HelloGL.h"

namespace
{
    // local pointer to the engine
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
            // draw the frame
            helloGL->Display();
        }
    }

    void Timer(int preferredRefresh)
    {
        if (helloGL != nullptr)
        {
            // run the update logic
            helloGL->Update();
        }

        // reset the timer for next time
        glutTimerFunc(preferredRefresh, GLUTCallbacks::Timer, preferredRefresh);
    }
}
