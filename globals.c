#include "globals.h"

float DELTA_TICKS = 0;
GLuint BOUND_TEXTURE = 0;

uint8_t BLOCK_SIZE = 48;

GLfloat cameraX = 0.0f, cameraY = 0.0f;
GLfloat mouseX = 0.0f, mouseY = 0.0f;

PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
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
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB = NULL;

GLuint shaderProgram = 0;
GLint colorLocation = 0;

GLuint texShader = 0;
GLint texPos = 0;
GLint texCoords = 0;
GLint texColor = 0;

int logIndex = 0;
char **logs = NULL;

void printLog(int type, const char *summary, const char *details)
{
	char message[256];

	switch(type)
	{
		case 0:
			// TODO: Replace "OK" and "ERROR" with colors.
			sprintf(message, "OK: %s%s", summary, details == NULL ? "" : details);
		break;

		case 1:
			sprintf(message, "ERROR: %s%s", summary, details == NULL ? "" : details);
		break;
	}
	strcpy(logs[logIndex++], message);
}
