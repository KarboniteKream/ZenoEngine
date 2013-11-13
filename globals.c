#include "globals.h"

float DELTA_TICKS = 0;

GLuint BOUND_TEXTURE = 0;
uint8_t BLOCK_SIZE = 48;

GLfloat cameraX = 0.0f, cameraY = 0.0f;
GLfloat mouseX = 0.0f, mouseY = 0.0f;

PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;

PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;

PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;

GLuint shaderProgram = 0;
GLint colorLocation = 0;

GLuint texShader = 0;
GLint texPos = 0;
GLint texCoords = 0;
GLint texUnit = 0;
GLint texColor = 0;

int logIndex = 0;
char **logString = NULL;
