#pragma once

class HelloGL;

namespace GLUTCallbacks
{
    // setup the class
    void Init(HelloGL* gl);

    // show stuff on screen
    void Display();

    // handle the timing 
    void Timer(int preferredRefresh);
}