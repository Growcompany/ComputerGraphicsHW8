#pragma once
#include <cstddef>
#include <glm/glm.hpp>

using namespace glm;

// Mesh globals declared in implementation
extern int   gNumVertices;
extern int   gNumTriangles;
extern vec3* gVertexArray;
extern int* gIndexBuffer;

// Generate unit-sphere mesh
void create_scene();