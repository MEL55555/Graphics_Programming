#include "GLUTCallbacks.h"
#include "HelloGL.h"

namespace
{
	// local pointer to the main class so we can call the display and update 
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
			// go draw the scene
            helloGL->Display();
        }
    }

    void Timer(int preferredRefresh)
    {
        if (helloGL != nullptr)
        {
            // run the actual game logic update
            helloGL->Update();
        }

        // set it up to run again so it keeps looping
        glutTimerFunc(preferredRefresh, GLUTCallbacks::Timer, preferredRefresh);
    }
}