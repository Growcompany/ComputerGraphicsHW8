/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "load_mesh.h"

float  					gTotalTimeElapsed = 0;
int 					gTotalFrames = 0;
GLuint 					gTimer;

void init_timer()
{
	glGenQueries(1, &gTimer);
}

void start_timing()
{
	glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
	glEndQuery(GL_TIME_ELAPSED);

	GLint available = GL_FALSE;
	while (available == GL_FALSE)
		glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);

	GLint result;
	glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);

	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
	return timeElapsed;
}

/*
	Your display function should look roughly like the following.
*/
void display()
{
	// TODO: Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer

	start_timing();

	// TODO: Draw the bunny.
	glBegin(GL_TRIANGLES); // �ﰢ����� - glVertex3f�� buffer�� 3���� ���̸� �����ؼ� �׷���  (immediate mode)
	for (auto& triangle : gTriangles) // load_mesh's gTriangles
	{
		// Triangle�� 3�� ���� ���� Normal, Vertex����
		for (int k = 0; k < 3; ++k)
		{
			unsigned int idx = triangle.indices[k];
			glNormal3f(gNormals[idx].x, gNormals[idx].y, gNormals[idx].z); // �� ���� ���� �������� ����
			glVertex3f(gPositions[idx].x, gPositions[idx].y, gPositions[idx].z); // Vertext�� �� ���� ��ġ ����
		}
		// 3�� ������ Normal, Vertex�� ����Ǹ� buffer�� �ִ� 3���� �̿��� �ﰢ���� �׸��� �ڵ����� �����

	}
	glEnd();


	float timeElapsed = stop_timing();
	gTotalFrames++;
	gTotalTimeElapsed += timeElapsed;
	float fps = gTotalFrames / gTotalTimeElapsed;
	char string[1024] = { 0 };
	sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
	glutSetWindowTitle(string);

	glutPostRedisplay();
	glutSwapBuffers();
}
