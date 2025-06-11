#pragma once

#include <GL/glew.h>

extern float  gTotalTimeElapsed;
extern int    gTotalFrames;
extern GLuint gTimer;
extern GLuint bunnyVAO;

void init_timer();
void start_timing();
float stop_timing();
void display();