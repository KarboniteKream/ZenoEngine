#include "globals.h"

float DELTA_TICKS = 0;

GLuint BOUND_TEXTURE = 0;
uint8_t BLOCK_SIZE = 48;

GLfloat cameraX = 0.0f, cameraY = 0.0f;
GLfloat mouseX = 0.0f, mouseY = 0.0f;

PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
