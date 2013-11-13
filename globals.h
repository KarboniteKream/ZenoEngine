#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#define NAME_VERSION "Zeno Engine 0.13.11.13"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CAMERA_SPEED 600.0f
#define EDITOR_EDGE 25.0f

#define COLLIDABLE 0

extern float DELTA_TICKS;
extern GLuint BOUND_TEXTURE;

extern uint8_t BLOCK_SIZE;

extern GLfloat cameraX, cameraY;
// NOTE: Should mouseX and mouseY be globals?
extern GLfloat mouseX, mouseY;

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

extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;

// TODO: Move to a separate struct/file.
extern GLuint shaderProgram;
extern GLint colorLocation;

extern GLuint texShader;
extern GLint texPos;
extern GLint texCoords;
extern GLint texColor;

extern int logIndex;
extern char **logs;

void printLog(int type, const char *error, const char *details);

#endif
