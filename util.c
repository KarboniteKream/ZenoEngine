#include "util.h"

void initWindow(SDL_Window **window, const char *windowTitle)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while initializing SDL.", NULL);
		exit(1);
	}

	atexit(SDL_Quit);

	// TODO: Support for borderless window.
	// TODO: Fullscreen support.
	*window = SDL_CreateWindow("Zeno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(*window == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while creating the window.", NULL);
		exit(1);
	}

	if(SDL_GL_CreateContext(*window) == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while creating the OpenGL context.", *window);
		exit(1);
	}

	loadExtensions();

	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetWindowIcon(*window, SDL_LoadBMP("images/icon.bmp"));
	SDL_ShowCursor(SDL_ENABLE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

	if(glGetError() != GL_NO_ERROR)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while initializing OpenGL.", *window);
		exit(1);
	}
}

// TODO: Rename 'extensions'.
void loadExtensions()
{
	// TODO: Check for supported extensions.
	glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
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
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	glUniform4f = (PFNGLUNIFORM4FPROC)SDL_GL_GetProcAddress("glUniform4f");
	glUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glIsShader = (PFNGLISSHADERPROC)SDL_GL_GetProcAddress("glIsShader");

	printLog(0, "Extensions loaded successfully.", NULL);
}

void executeCommand(Level *level, const char *command)
{
	if(strlen(command) > 0)
	{
		char temp[strlen(command) + 1];
		strcpy(temp, command);

		char **commandArray = (char **)malloc(sizeof(char *));
		int index = 0, length = 0;
		commandArray[index] = strtok(temp, " ");

		while(commandArray[index] != NULL)
		{
			index++;
			length++;
			commandArray = (char **)realloc(commandArray, (index + 1) * sizeof(char *));
			commandArray[index] = strtok(NULL, " ");
		}

		// TODO: Add error reporting.
		if(strcmp(commandArray[0], "save") == 0)
		{
			if(length > 1)
			{
				if(length > 2 && strcmp(commandArray[1], "level") == 0)
				{
					saveLevel(level, commandArray[2]);
				}
			}
		}
		else if(strcmp(commandArray[0], "load") == 0)
		{
			if(length > 1)
			{
				if(length > 2 && strcmp(commandArray[1], "level") == 0)
				{
					loadLevel(level, commandArray[2]);
				}
			}
		}
		else if(strcmp(commandArray[0], "set") == 0)
		{
			if(length > 1)
			{
				// TODO: This should be removed some day.
				if(length > 2 && strcmp(commandArray[1], "scale") == 0)
				{
					BLOCK_SIZE = atoi(commandArray[2]);
					// TODO: Reload the level with the new textures.
				}
			}
		}
		else if(strcmp(commandArray[0], "debug") == 0)
		{
			level->Debug = !level->Debug;
		}
		else if(strcmp(commandArray[0], "vsync") == 0)
		{
			SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());
		}

		free(commandArray);
	}
}

void saveScreenshot()
{
	Uint32 rMask, gMask, bMask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rMask = 0xFF0000;
		gMask = 0x00FF00;
		bMask = 0x0000FF;
	#else
		rMask = 0x0000FF;
		gMask = 0x00FF00;
		bMask = 0xFF0000;
	#endif

	SDL_Surface *screenshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, rMask, gMask, bMask, 0);

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

	// TODO: Error checking.
	printLog(0, "Screenshot saved successfully as ", "'screenshots/screenshot.png'.");
}

void loadShader(GLuint *shaderProgram, const char *vsFilename, const char *fsFilename)
{
	FILE *vsFile = fopen(vsFilename, "r");
	FILE *fsFile = fopen(fsFilename, "r");

	if(vsFile == NULL)
	{
		printLog(1, "Unable to open vertex shader: ", vsFilename);
	}
	else if(fsFile == NULL)
	{
		printLog(1, "Unable to open fragment shader: ", fsFilename);
	}
	else
	{
		// NOTE: Should fopen() have an argument "a", so there is no need to fseek()?
		fseek(vsFile, 0, SEEK_END);
		fseek(fsFile, 0, SEEK_END);

		unsigned int vsLength = ftell(vsFile);
		unsigned int fsLength = ftell(fsFile);

		fseek(vsFile, 0, SEEK_SET);
		fseek(fsFile, 0, SEEK_SET);

		GLchar **vsSource = (GLchar **)malloc(sizeof(GLchar *));
		vsSource[0] = (GLchar *)calloc(vsLength + 1, sizeof(GLchar));

		GLchar **fsSource = (GLchar **)malloc(sizeof(GLchar *));
		fsSource[0] = (GLchar *)calloc(fsLength + 1, sizeof(GLchar));

		fread(vsSource[0], sizeof(GLchar), vsLength, vsFile);
		fread(fsSource[0], sizeof(GLchar), fsLength, fsFile);

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const GLchar **)vsSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, (const GLchar **)fsSource, NULL);
		glCompileShader(fragmentShader);

		GLint compileStatus;

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printLog(1, "Unable to compile vertex shader: ", vsFilename);
			printInfoLog(vertexShader);
		}

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printLog(1, "Unable to compile fragment shader: ", fsFilename);
			printInfoLog(fragmentShader);
		}

		if(compileStatus == GL_TRUE)
		{
			glUseProgram(*shaderProgram);
			*shaderProgram = glCreateProgram();

			glAttachShader(*shaderProgram, vertexShader);
			glAttachShader(*shaderProgram, fragmentShader);

			glLinkProgram(*shaderProgram);

			GLint linkStatus;
			glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &linkStatus);

			if(linkStatus == GL_FALSE)
			{
				printInfoLog(*shaderProgram);
			}

			glUseProgram(0);
		}

		free(vsSource);
		free(fsSource);
	}

	fclose(vsFile);
	fclose(fsFile);
}

void printInfoLog(GLuint shaderProgram)
{
	GLsizei maxLength, length = 0;
	GLboolean isShader = glIsShader(shaderProgram);

	if(isShader == GL_TRUE)
	{
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
	}
	else
	{
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
	}

	char *infoLog = (char *)malloc(maxLength * sizeof(char));

	if(isShader == GL_TRUE)
	{
		glGetShaderInfoLog(shaderProgram, maxLength, &length, infoLog);
	}
	else
	{
		glGetProgramInfoLog(shaderProgram, maxLength, &length, infoLog);
	}

	if(length > 0)
	{
		printLog(1, infoLog, NULL);
	}

	free(infoLog);
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

void drawEmptyRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat lineWidth, GLfloat r, GLfloat g, GLfloat b)
{
	glUseProgram(shaderProgram);
	glUniform4f(colorLocation, r, g, b, 1.0f);

	glLineWidth(lineWidth);

	Coordinate vertexData[8];

	vertexData[0].X = x;
	vertexData[0].Y = y;

	vertexData[1].X = x + w;
	vertexData[1].Y = y;

	vertexData[2].X = x + w - 1.0f;
	vertexData[2].Y = y;

	vertexData[3].X = x + w - 1.0f;
	vertexData[3].Y = y + h;

	vertexData[4].X = x + w - 1.0f;
	vertexData[4].Y = y + h - 1.0f;

	vertexData[5].X = x;
	vertexData[5].Y = y + h - 1.0f;

	vertexData[6].X = x + 1.0f;
	vertexData[6].Y = y + h - 1.0f;

	vertexData[7].X = x + 1.0f;
	vertexData[7].Y = y;

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertexData);
	glDrawArrays(GL_LINES, 0, 8);

	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1.0f);

	glUseProgram(0);
}
