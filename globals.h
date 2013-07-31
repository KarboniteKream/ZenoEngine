#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// NOTE: Should this rather be read from a file?
#define NAME_VERSION "Zeno Engine 0.2.7w"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CAMERA_SPEED 600.0f
#define EDITOR_EDGE 20.0f

#define COLLIDABLE 0

extern float DELTA_TICKS;

extern GLuint BOUND_TEXTURE;
extern uint8_t BLOCK_SIZE;

extern GLfloat cameraX, cameraY;
// NOTE: Should mouseX and mouseY be globals?
extern GLfloat mouseX, mouseY;

typedef struct
{
	GLfloat X;
	GLfloat Y;
	GLfloat W;
	GLfloat H;
} Rectangle;

typedef struct
{
	GLfloat X;
	GLfloat Y;
} Coordinate;

typedef struct
{
	GLfloat X;
	GLfloat Y;
	GLfloat TX;
	GLfloat TY;
} VertexData;

extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

#endif
