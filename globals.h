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

typedef void (APIENTRY * GL_BindBuffer)(GLenum, GLuint);
typedef void (APIENTRY * GL_GenBuffers)(GLsizei, GLuint *);
typedef void (APIENTRY * GL_DeleteBuffers)(GLsizei, const GLuint *);
typedef void (APIENTRY * GL_BufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void (APIENTRY * GL_BufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid *);

typedef void (APIENTRY * GL_UseProgram)(GLuint);
typedef GLuint (APIENTRY * GL_CreateProgram)(void);
typedef void (APIENTRY * GL_DeleteProgram)(GLuint);
typedef void (APIENTRY * GL_LinkProgram)(GLuint);
typedef void (APIENTRY * GL_GetProgramiv)(GLuint, GLenum, GLint *);
typedef void (APIENTRY * GL_GetProgramInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *);

typedef GLuint (APIENTRY * GL_CreateShader)(GLenum);
typedef void (APIENTRY * GL_DeleteShader)(GLuint);
typedef void (APIENTRY * GL_ShaderSource)(GLuint, GLsizei, const GLchar * const *, const GLint *);
typedef void (APIENTRY * GL_CompileShader)(GLuint);
typedef GLboolean (APIENTRY * GL_IsShader)(GLuint);
typedef void (APIENTRY * GL_GetShaderiv)(GLuint, GLenum, GLint *);
typedef void (APIENTRY * GL_GetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (APIENTRY * GL_AttachShader)(GLuint, GLuint);
typedef void (APIENTRY * GL_DetachShader)(GLuint, GLuint);

typedef GLint (APIENTRY * GL_GetAttribLocation)(GLuint, const GLchar *);
typedef GLint (APIENTRY * GL_GetUniformLocation)(GLuint, const GLchar *);
typedef void (APIENTRY * GL_Uniform1i)(GLint, GLint);
typedef void (APIENTRY * GL_Uniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);

typedef void (APIENTRY * GL_EnableVertexAttribArray)(GLuint);
typedef void (APIENTRY * GL_DisableVertexAttribArray)(GLuint);
typedef void (APIENTRY * GL_VertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);

GL_BindBuffer glBindBuffer;
GL_GenBuffers glGenBuffers;
GL_DeleteBuffers glDeleteBuffers;
GL_BufferData glBufferData;
GL_BufferSubData glBufferSubData;

GL_UseProgram glUseProgram;
GL_CreateProgram glCreateProgram;
GL_DeleteProgram glDeleteProgram;
GL_LinkProgram glLinkProgram;
GL_GetProgramiv glGetProgramiv;
GL_GetProgramInfoLog glGetProgramInfoLog;

GL_CreateShader glCreateShader;
GL_DeleteShader glDeleteShader;
GL_ShaderSource glShaderSource;
GL_CompileShader glCompileShader;
GL_IsShader glIsShader;
GL_GetShaderiv glGetShaderiv;
GL_GetShaderInfoLog glGetShaderInfoLog;
GL_AttachShader glAttachShader;
GL_DetachShader glDetachShader;

GL_GetAttribLocation glGetAttribLocation;
GL_GetUniformLocation glGetUniformLocation;
GL_Uniform1i glUniform1i;
GL_Uniform4f glUniform4f;

GL_EnableVertexAttribArray glEnableVertexAttribArray;
GL_DisableVertexAttribArray glDisableVertexAttribArray;
GL_VertexAttribPointer glVertexAttribPointer;

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
