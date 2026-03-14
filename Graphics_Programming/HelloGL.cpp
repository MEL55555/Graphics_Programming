#include "HelloGL.h"

float camX = 0.0f;
float camY = 0.0f;
float camZ = -8.0f;

float rotX = 0.0f;
float rotY = 0.0f;

float cubeRotation = 0.0f;

int lastMouseX = 400;
int lastMouseY = 300;


// KEYBOARD CONTROLS
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': camZ += 0.3f; break;
	case 's': camZ -= 0.3f; break;
	case 'a': camX += 0.3f; break;
	case 'd': camX -= 0.3f; break;
	}
}


// MOUSE LOOK
void MouseMotion(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;

	rotY += dx * 0.2f;
	rotX += dy * 0.2f;

	lastMouseX = x;
	lastMouseY = y;

	glutPostRedisplay();
}



HelloGL::~HelloGL(void)
{
}



HelloGL::HelloGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Edgar's Graphics Programming Project");

	glClearColor(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutDisplayFunc(GLUTCallbacks::Display);
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(MouseMotion);

	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);

	glutMainLoop();
}



void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);

	glTranslatef(camX, camY, camZ);

	// ROTATING CUBE
	glPushMatrix();
	glRotatef(cubeRotation, 1, 1, 0);
	DrawIndexedCubeAlt();
	glPopMatrix();

	// PYRAMID
	glPushMatrix();
	glTranslatef(4, 0, 0);
	DrawPyramid();
	glPopMatrix();

	glFlush();
}



void HelloGL::Update()
{
	cubeRotation += 0.5f;

	if (cubeRotation >= 360)
		cubeRotation = 0;

	glutPostRedisplay();
}



// INDEXED CUBE VERTICES
HelloGL::Vertex HelloGL::indexedVertices[] =
{
	{1,1,1},{-1,1,1},
	{-1,-1,1},{1,-1,1},
	{1,-1,-1},{1,1,-1},
	{-1,1,-1},{-1,-1,-1}
};



HelloGL::Color HelloGL::indexedColors[] =
{
	{1,1,1},{1,1,0},
	{1,0,0},{1,0,1},
	{0,0,1},{0,1,1},
	{0,1,0},{0,0,0}
};



GLushort HelloGL::indices[] =
{
	0,1,2, 2,3,0,
	0,3,4, 4,5,0,
	0,5,6, 6,1,0,
	1,6,7, 7,2,1,
	7,4,3, 3,2,7,
	4,7,6, 6,5,4
};



// 
void HelloGL::DrawIndexedCubeAlt()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, indexedVertices);
	glColorPointer(3, GL_FLOAT, 0, indexedColors);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}



// PYRAMID
void HelloGL::DrawPyramid()
{
	glBegin(GL_TRIANGLES);

	// front
	glColor3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);

	// right
	glColor3f(0, 1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);

	// back
	glColor3f(0, 0, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);

	// left
	glColor3f(1, 1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);

	glEnd();

	// base
	glBegin(GL_QUADS);

	glColor3f(1, 0, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);

	glEnd();
}




void HelloGL::DrawCube() {}
void HelloGL::DrawCubeArray() {}