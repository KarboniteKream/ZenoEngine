#include "globals.h"

float DELTA_TICKS = 0;
GLuint BOUND_TEXTURE = 0;

uint8_t BLOCK_SIZE = 48;

bool DEBUG = false;
bool MULTIPLAYER = false;
bool CLIENT = false;

UDPsocket SOCKET = NULL;
IPaddress ADDRESS;
UDPpacket *PACKET = NULL;

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;

GLfloat mouseX = 0.0f;
GLfloat mouseY = 0.0f;

GLuint shaderProgram = 0;
GLint colorLocation = 0;

GLuint texShader = 0;
GLint texPos = 0;
GLint texCoords = 0;
GLint texColor = 0;

int logIndex = 0;

// TODO: Better argument handling, variable number of arguments.
void printLog(int type, const char *summary, const char *details)
{
	// TODO: Dynamic size.
	// TODO: Free logs.
	logs[logIndex] = (char *)malloc(256 * sizeof(char));
	// NOTE: Should details be just another message, but smaller, or with a different color?
	char message[256];

	// TODO: Replace "OK" and "ERROR" with colors.
	// TODO: Remove NULL.
	switch(type)
	{
		case 0:
			snprintf(message, 256, "OK: %s %s", summary, (details == NULL) ? "" : details);
		break;

		case 1:
			snprintf(message, 256, "ERROR: %s %s", summary, (details == NULL) ? "" : details);
			fprintf(stderr, "%s\n", message);
			strcpy(logs[logIndex++], message);
		break;

		case 2:
			snprintf(message, 256, "%s %s", summary, (details == NULL) ? "" : details);
			strcpy(logs[logIndex++], message);
		break;
	}

	// TODO: Open file only once.
	FILE *logFile = fopen("log.txt", "a");

	if(logFile != NULL)
	{
		fprintf(logFile, "%s\n", message);
	}

	fclose(logFile);
}

// TODO: Rename and move to a separate namespace.
FILE *openFile(const char *filename, const char *mode)
{
	char *path = SDL_GetBasePath();
	strcat(path, filename);

	printLog(0, "Opening file", filename);
	printLog(0, "Path:", path);
	// TODO: Fix Unicode support.
	FILE *file = fopen(path, mode);

	// NOTE: Free?

	return file;
}
