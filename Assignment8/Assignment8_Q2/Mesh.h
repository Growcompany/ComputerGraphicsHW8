#pragma once

#include <GL/glew.h>
#include <vector>

struct RenderData {
    GLuint VAO;
    GLuint VBO_positions;
    GLuint VBO_normals;
    GLuint EBO;
    std::vector<unsigned int> indices;
};