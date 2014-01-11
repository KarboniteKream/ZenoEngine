#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#define NAME_VERSION "Zeno Engine 0.13.11.27"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CAMERA_SPEED 600.0f
#define EDITOR_EDGE 25.0f

// TODO: Remove?
#define LAYOUT 0
#define PROPERTIES 1
#define COLLIDABLE 1
#define SLOPE_LEFT 2

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

PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;

PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLISSHADERPROC glIsShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;

// TODO: Move to a separate struct/file.
extern GLuint shaderProgram;
extern GLint colorLocation;

extern GLuint texShader;
extern GLint texPos;
extern GLint texCoords;
extern GLint texColor;

extern int logIndex;
char *logs[256];

void printLog(int type, const char *error, const char *details);

#endif
