#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include "load_mesh.h"
#include "frame_timer.h"

using namespace glm;

int nx = 1280;
int ny = 1280;

void ParametersSetup()
{
    // depth buffer setting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Backface-culling Off
    glDisable(GL_CULL_FACE);

    // Normalize Option
    glEnable(GL_NORMALIZE);

    // Viewport setting
    glViewport(0, 0, nx, ny);

    // Perspective Projection setting
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    // Model View 행렬 setting
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // 카메라가 e = (0, 0, 0), w = (0, 0, 1) 이므로 identity

    // 모델 변환 1.scale 2.translate
    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    // Global Ambient set
    GLfloat GlobalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalAmbient);

    // 재질 설정
    GLfloat Ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat Kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat Ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat Shininess = 0.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);

    // Light setting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat LightDir[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat LightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };     // White
    GLfloat LightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };       

    glLightfv(GL_LIGHT0, GL_POSITION, LightDir);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(nx, ny);
    glutCreateWindow("OpenGL Bunny");

    // GLEW Initialize
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    load_mesh("bunny.obj");
    ParametersSetup();

    init_timer();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}