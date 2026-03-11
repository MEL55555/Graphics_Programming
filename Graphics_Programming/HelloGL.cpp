#include "HelloGL.h"
HelloGL::~HelloGL(void)
{
}
HelloGL::HelloGL(int argc, char* argv[])
{
	rotationRect = 0.0f;
	rotationSquare = 0.0f;
	rotationTriangle = 0.0f;
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Game Loop Tutorial");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glutMainLoop();
}
void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawRectangle();
	DrawSquare();
	DrawTriangle();
	glFlush();
}
void HelloGL::Update()
{
	rotationRect += 0.5f;
	if (rotationRect >= 360.0f)
		rotationRect = 0.0f;
	rotationSquare += 1.0f;
	if (rotationSquare >= 360.0f)
		rotationSquare = 0.0f;
	rotationTriangle -= 0.7f;
	if (rotationTriangle <= -360.0f)
		rotationTriangle = 0.0f;
	glutPostRedisplay();
}
void HelloGL::DrawRectangle()
{
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.0f);
	glRotatef(rotationRect, 0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-0.4f, 0.2f);
	glVertex2f(0.4f, 0.2f);
	glVertex2f(0.4f, -0.2f);
	glVertex2f(-0.4f, -0.2f);
	glEnd();
	glPopMatrix();
}
void HelloGL::DrawSquare()
{
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.0f);
	glRotatef(rotationSquare, 0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3f, 0.3f);
	glVertex2f(0.3f, 0.3f);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(-0.3f, -0.3f);
	glEnd();
	glPopMatrix();
}
void HelloGL::DrawTriangle()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.6f, 0.0f);
	glRotatef(rotationTriangle, 0.0f, 0.0f, -1.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.4f);
	glVertex2f(-0.4f, -0.4f);
	glVertex2f(0.4f, -0.4f);
	glEnd();
	glPopMatrix();
}