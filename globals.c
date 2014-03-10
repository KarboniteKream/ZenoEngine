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
			sprintf(message, "OK: %s %s", summary, (details == NULL) ? "" : details);
		break;

		case 1:
			sprintf(message, "ERROR: %s %s", summary, (details == NULL) ? "" : details);
		break;
	}

	strcpy(logs[logIndex++], message);

	if(type == 1)
	{
		printf("%s\n", message);

		FILE *errors = fopen("errors.txt", "w");
		fprintf(errors, "%s\n", message);
		fclose(errors);
	}
}
