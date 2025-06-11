// Immediate-Mode Rendering of Bunny (Q1)
// Combines load_mesh.cpp + frame_timer.cpp for OpenGL immediate-mode
environment

#include <GL/glew.h>
#include <GL/glut.h>
#include "load_mesh.cpp"      // Provides load_mesh(), gPositions, gNormals, gTriangles
#include "frame_timer.cpp"    // Provides init_timer(), start_timing(), stop_timing(), FPS logic

// Initialize OpenGL state (depth test, lighting, material)
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Ambient light
    GLfloat Ia[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    // Diffuse light
    GLfloat Ld[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // Directional light vector (w=0)
    GLfloat lightPos[4] = { -1.0f, -1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Ld);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Material: ka = kd = (1,1,1)
    GLfloat matAD[4] = { 1.0f,1.0f,1.0f,1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAD);
}

// Reshape callback: setup viewport & projection
void reshape(int w, int h) {
    // Fixed viewport of 1280x1280
    glViewport(0, 0, 1280, 1280);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspective frustum: left, right, bottom, top, near, far
    // Note: OpenGL expects positive near/far values
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

// Display callback: clear, draw mesh in immediate mode, measure time
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    start_timing();

    glLoadIdentity();
    // Model transform: scale then translate
    glScalef(10.0f, 10.0f, 10.0f);
    glTranslatef(0.1f, -1.0f, -1.5f);

    // Immediate-mode draw
    glBegin(GL_TRIANGLES);
    for (const auto& tri : gTriangles) {
        for (int i = 0; i < 3; ++i) {
            Vector3 n = gNormals[tri.indices[i]];
            glNormal3f(n.x, n.y, n.z);
            Vector3 v = gPositions[tri.indices[i]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    float elapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += elapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[256];
    sprintf(title, "OpenGL Bunny Q1: %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay();  // continuously render
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny Q1");

    // Initialize GLEW (after creating context)
    glewInit();

    // Load mesh data
    load_mesh("bunny.obj");

    // Setup timer query
    init_timer();

    // OpenGL state
    initGL();

    // Callbacks
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    // Enter main loop
    glutMainLoop();
    return 0;
}