#include <Windows.h>
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

#include "sphere_scene.h"

using namespace glm;

GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
            vertex_file_path.c_str());
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path.c_str());
    const char* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path.c_str());
    const char* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

void Update_mesh(const GLuint& VAO, const std::vector<GLuint>& GLBuffers,
    std::vector<glm::vec3>& Positions,
    std::vector<glm::vec3>& Normals,
    std::vector<glm::vec3>& Colors,
    std::vector<unsigned int>& Indices)
{
    glBindVertexArray(VAO); // must bind before binding buffers

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Positions[0]), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Colors[0]), &Colors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO to avoid accidental modification
}

// Global Variables
int Width = 512;
int Height = 512;

void resize_callback(GLFWwindow* window, int nw, int nh)
{
    Width = nw;
    Height = nh;
    // Tell the viewport to use all of our screen estate
    glViewport(0, 0, nw, nh);
}

void processInput(GLFWwindow* window)
{
    // Close when the user hits 'q' or escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char* argv[])
{
    // Initialize Window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);                             // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);               // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);       // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);      // Turn on face culling
    glCullFace(GL_BACK);         // Cull back faces
    glFrontFace(GL_CCW);         // Triangles with counter‐clockwise winding are front faces

    // Create and compile our GLSL program from the shaders
    GLuint shaderProgram = LoadShaders("Phong.vert", "Phong.frag");
    glUseProgram(shaderProgram);

    GLint loc_lightPos = glGetUniformLocation(shaderProgram, "u_lightPos");
    GLint loc_eyePos = glGetUniformLocation(shaderProgram, "u_eyePos");
    GLint loc_lightColor = glGetUniformLocation(shaderProgram, "u_lightColor");
    GLint loc_ka = glGetUniformLocation(shaderProgram, "u_ka");
    GLint loc_kd = glGetUniformLocation(shaderProgram, "u_kd");
    GLint loc_ks = glGetUniformLocation(shaderProgram, "u_ks");
    GLint loc_p = glGetUniformLocation(shaderProgram, "u_p");
    GLint loc_Ia = glGetUniformLocation(shaderProgram, "u_Ia");
    GLint loc_gamma = glGetUniformLocation(shaderProgram, "u_gamma");
    GLint loc_model = glGetUniformLocation(shaderProgram, "model");
    GLint loc_view = glGetUniformLocation(shaderProgram, "view");
    GLint loc_proj = glGetUniformLocation(shaderProgram, "proj");

    glm::vec3 ka = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 kd = glm::vec3(0.0f, 0.5f, 0.0f);
    glm::vec3 ks = glm::vec3(0.5f, 0.5f, 0.5f);
    float p = 32.0f;
    float Ia = 0.2f;
    glm::vec3 lightPos = glm::vec3(-4.0f, 4.0f, -3.0f);
    float gammaVal = 2.2f;
    glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), vec3(0, 0, -7)) * glm::scale(glm::mat4(1.0f), vec3(2.0f));
    glm::mat4 view = glm::lookAt(vec3(0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	// Create Sphere Scene
    create_scene();

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec3> Colors;
    std::vector<unsigned int> Indices;

    Positions.reserve(gNumVertices);
    Normals.reserve(gNumVertices);
    for (int i = 0; i < gNumVertices; ++i) {
        Positions.push_back(gVertexArray[i]);
        Normals.push_back(normalize(gVertexArray[i]));
    }

    Colors.resize(gNumVertices);
    for (int i = 0; i < gNumVertices; ++i) {
        Colors[i] = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    Indices.reserve(3 * gNumTriangles);
    for (int i = 0; i < 3 * gNumTriangles; ++i) {
        Indices.push_back(static_cast<unsigned int>(gIndexBuffer[i]));
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    const int numBuffers = 4; // Buffers for Positions, Normals, Colors, Indices
    std::vector<GLuint> GLBuffers;
    GLBuffers.resize(numBuffers, 0);
    glGenBuffers(numBuffers, &GLBuffers[0]);
    Update_mesh(VAO, GLBuffers, Positions, Normals, Colors, Indices);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(loc_model, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(loc_proj, 1, GL_FALSE, &proj[0][0]);

        glUniform3fv(loc_ka, 1, &ka[0]);
        glUniform3fv(loc_kd, 1, &kd[0]);
        glUniform3fv(loc_ks, 1, &ks[0]);
        glUniform1f(loc_p, p);
        glUniform1f(loc_Ia, Ia);
        glUniform3fv(loc_lightPos, 1, &lightPos[0]);
        glUniform1f(loc_gamma, gammaVal);
        glUniform3fv(loc_eyePos, 1, &eyePos[0]);
        glUniform3f(loc_lightColor, 1.0f, 1.0f, 1.0f);

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(Indices.size()),
            GL_UNSIGNED_INT,
            0
        );
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    if (GLBuffers[0] != 0)
        glDeleteBuffers(numBuffers, &GLBuffers[0]);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



