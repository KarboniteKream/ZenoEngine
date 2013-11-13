#include "util.h"

void loadExtensions()
{
	// TODO: Check for supported extensions.
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBufferARB");
	glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
	glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)SDL_GL_GetProcAddress("glBufferSubData");

	glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	glUniform4f = (PFNGLUNIFORM4FPROC)SDL_GL_GetProcAddress("glUniform4f");

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
}

void drawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glUseProgram(shaderProgram);
	glUniform4f(colorLocation, r, g, b, a);

	glEnableClientState(GL_VERTEX_ARRAY);

	Coordinate vertexData[4];

	vertexData[0].X = x;
	vertexData[0].Y = y;

	vertexData[1].X = x + w;
	vertexData[1].Y = y;

	vertexData[2].X = x + w;
	vertexData[2].Y = y + h;

	vertexData[3].X = x;
	vertexData[3].Y = y + h;

	glVertexPointer(2, GL_FLOAT, 0, vertexData);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);

	glUseProgram(0);
}

void drawEmptyRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth, GLfloat r, GLfloat g, GLfloat b)
{
	glUseProgram(shaderProgram);
	glUniform4f(colorLocation, r, g, b, 1.0f);

	glLineWidth(lineWidth);

	Coordinate vertexData[8];

	vertexData[0].X = x1;
	vertexData[0].Y = y1;

	vertexData[1].X = x2;
	vertexData[1].Y = y1;

	vertexData[2].X = x2 - 1.0f;
	vertexData[2].Y = y1;

	vertexData[3].X = x2 - 1.0f;
	vertexData[3].Y = y2;

	vertexData[4].X = x2 - 1.0f;
	vertexData[4].Y = y2 - 1.0f;

	vertexData[5].X = x1;
	vertexData[5].Y = y2 - 1.0f;

	vertexData[6].X = x1 + 1.0f;
	vertexData[6].Y = y2 - 1.0f;

	vertexData[7].X = x1 + 1.0f;
	vertexData[7].Y = y1;

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertexData);
	glDrawArrays(GL_LINES, 0, 8);

	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1.0f);

	glUseProgram(0);
}

void saveScreenshot()
{
	SDL_Surface *screenshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);

	unsigned char *pixels = (unsigned char *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
	glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	for(int i = 0; i < SCREEN_HEIGHT; i++)
	{
		memcpy(screenshot->pixels + SCREEN_WIDTH * 3 * i, pixels + SCREEN_WIDTH * 3 * (SCREEN_HEIGHT - i - 1), SCREEN_WIDTH * 3);
	}

	// TODO: Add automatic numbering.
	IMG_SavePNG(screenshot, "screenshots/screenshot.png");

	free(pixels);
	free(screenshot);
}

void loadShader(GLuint *shaderProgram, const char *vertexFilename, const char *fragmentFilename)
{
	FILE *vertexFile = fopen(vertexFilename, "rb");
	FILE *fragmentFile = fopen(fragmentFilename, "rb");

	if(vertexFile != NULL && fragmentFile != NULL)
	{
		fseek(vertexFile, 0, SEEK_END);
		fseek(fragmentFile, 0, SEEK_END);

		unsigned int vertexLength = ftell(vertexFile);
		unsigned int fragmentLength = ftell(fragmentFile);

		fseek(vertexFile, 0, SEEK_SET);
		fseek(fragmentFile, 0, SEEK_SET);

		GLchar **vertexSource = (GLchar **)malloc(sizeof(GLchar *));
		vertexSource[0] = (GLchar *)calloc(vertexLength + 1, sizeof(GLchar));

		GLchar **fragmentSource = (GLchar **)malloc(sizeof(GLchar *));
		fragmentSource[0] = (GLchar *)calloc(fragmentLength + 1, sizeof(GLchar));

		fread(vertexSource[0], sizeof(GLchar), vertexLength, vertexFile);
		fread(fragmentSource[0], sizeof(GLchar), fragmentLength, fragmentFile);

		glUseProgram(*shaderProgram);
		*shaderProgram = glCreateProgram();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const GLchar **)vertexSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, (const GLchar **)fragmentSource, NULL);
		glCompileShader(fragmentShader);

		GLint compileStatus = GL_FALSE;

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printf("Unable to compile vertex shader '%s'\n", vertexFilename);
			printShaderLog(vertexShader);
		}

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printf("Unable to compile fragment shader '%s'\n", fragmentFilename);
			printShaderLog(fragmentShader);
		}

		if(compileStatus == GL_FALSE)
		{
			glDeleteProgram(*shaderProgram);
			return;
		}

		glAttachShader(*shaderProgram, vertexShader);
		glAttachShader(*shaderProgram, fragmentShader);

		// TODO: Check for error when linking.
		glLinkProgram(*shaderProgram);
	}
}

void printShaderLog(GLuint shader)
{
	int infoLogLength = 0, maxLength;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	char *infoLog = (char *)malloc(maxLength * sizeof(char));

	glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

	if(infoLogLength > 0)
	{
		printf("%s", infoLog);
		printLog(logString, infoLog);
	}
}

void printLog(char **logString, const char *text)
{
	strcpy(logString[logIndex++], text);
}
