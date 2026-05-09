#pragma once

class HelloGL;

namespace GLUTCallbacks
{
    // getting the class ready
    void Init(HelloGL* gl);

    // put everything on the screen
    void Display();

    // makes sure the game runs at the right speed
    void Timer(int preferredRefresh);
}