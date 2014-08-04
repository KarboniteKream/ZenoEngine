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

// FIXME: Rectangle conflicts with Win32API. @windows
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

typedef void (APIENTRY * GL_BindBuffer)(GLenum target, GLuint buffer);
typedef void (APIENTRY * GL_GenBuffers)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY * GL_DeleteBuffers)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * GL_BufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * GL_BufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

typedef void (APIENTRY * GL_UseProgram)(GLuint program);
typedef GLuint (APIENTRY * GL_CreateProgram)();
typedef void (APIENTRY * GL_DeleteProgram)(GLuint program);
typedef void (APIENTRY * GL_LinkProgram)(GLuint program);
typedef void (APIENTRY * GL_GetProgramiv)(GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY * GL_GetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

typedef GLuint (APIENTRY * GL_CreateShader)(GLenum shaderType);
typedef void (APIENTRY * GL_DeleteShader)(GLuint shader);
typedef void (APIENTRY * GL_ShaderSource)(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length);
typedef void (APIENTRY * GL_CompileShader)(GLuint shader);
typedef GLboolean (APIENTRY * GL_IsShader)(GLuint shader);
typedef void (APIENTRY * GL_GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY * GL_GetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY * GL_AttachShader)(GLuint program, GLuint shader);
typedef void (APIENTRY * GL_DetachShader)(GLuint program, GLuint shader);

typedef GLint (APIENTRY * GL_GetAttribLocation)(GLuint program, const GLchar *name);
typedef GLint (APIENTRY * GL_GetUniformLocation)(GLuint program, const GLchar *name);
typedef void (APIENTRY * GL_Uniform1i)(GLint location, GLint v0);
typedef void (APIENTRY * GL_Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

typedef void (APIENTRY * GL_EnableVertexAttribArray)(GLuint index);
typedef void (APIENTRY * GL_DisableVertexAttribArray)(GLuint index);
typedef void (APIENTRY * GL_VertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

// TODO: Initialize to NULL.
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
FILE *openFile(const char *filename, const char *mode);

#endif
