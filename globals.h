#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#define NAME_VERSION "Zeno Engine 0.14.01.11"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CAMERA_SPEED 600.0f
#define EDITOR_EDGE 25.0f

// TODO: Remove?
#define LAYOUT 0
#define PROPERTIES 1
#define COLLIDABLE 1
#define SLOPE_LEFT 2
#define SLOPE_RIGHT 3

extern float DELTA_TICKS;
extern GLuint BOUND_TEXTURE;

// TODO: Replace with a #define.
extern uint8_t BLOCK_SIZE;

extern bool DEBUG;
extern bool MULTIPLAYER;
extern bool CLIENT;

extern UDPsocket SOCKET;
extern IPaddress ADDRESS;
extern UDPpacket *PACKET;

extern GLfloat cameraX;
extern GLfloat cameraY;

// NOTE: Should mouseX and mouseY be globals?
extern GLfloat mouseX;
extern GLfloat mouseY;

// FIXME: Rectangle conflicts with Win32API.
typedef struct
{
	GLfloat X;
	GLfloat Y;
	GLfloat W;
	GLfloat H;
} RectangleF;

typedef struct
{
	GLfloat X;
	GLfloat Y;
} Coordinate;

typedef struct
{
	GLfloat X;
	GLfloat Y;
	GLfloat S;
	GLfloat T;
} VertexData;

// TODO: Move to a separate struct/file.
// TODO: Add dynamic loading.
extern GLuint shaderProgram;
extern GLint colorLocation;

extern GLuint texShader;
extern GLint texPos;
extern GLint texCoords;
extern GLint texColor;

extern int logIndex;
char *logs[256];

void printLog(int type, const char *summary, const char *details);

#endif
