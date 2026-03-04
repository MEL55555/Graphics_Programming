#include "HelloGL.h"
HelloGL::~HelloGL(void)
{

}

void HelloGL::Display()
{

	glClear(GL_COLOR_BUFFER_BIT); //this clears the scene glBegin(GL_POLYGON); //starts to draw a polygon
	glVertex2f(-0.75, 0.5); //define the first point of the polygon, top left glVertex2f(0.75, 0.5); //next point, top right
	glVertex2f(0.75, -0.5); //bottom right
	glVertex2f(-0.75, -0.5); //last point of the polygon, bottom left
	glEnd(); // defines the end of the draw
	glFlush(); //flushes the scene drawn to the graphics card
}

HelloGL::HelloGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutCreateWindow("Simple OpenGL Program"); 
	glutDisplayFunc(GLUTCallbacks::Display);
	glutMainLoop();
}

//finsihed





