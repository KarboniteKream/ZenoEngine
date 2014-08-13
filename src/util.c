#include "util.h"

void initWindow(SDL_Window **window, const char *windowTitle)
{
	// TODO: Support for SDL_GetError() and SDLNet_GetError().
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while initializing SDL.", NULL);
		exit(1);
	}

	if(SDLNet_Init() == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while initializing SDL_net.", NULL);
		SDL_Quit();
		exit(1);
	}

	// TODO: Setup atexit().

	// TODO: Support for borderless window.
	// TODO: Fullscreen support.
	*window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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
		// TODO: Call printLog().
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred while initializing OpenGL.", *window);
		exit(1);
	}
}

// TODO: Rename 'extensions'.
void loadExtensions()
{
	glBindBuffer = (GL_BindBuffer)SDL_GL_GetProcAddress("glBindBuffer");
	glGenBuffers = (GL_GenBuffers)SDL_GL_GetProcAddress("glGenBuffers");
	glDeleteBuffers = (GL_DeleteBuffers)SDL_GL_GetProcAddress("glDeleteBuffers");
	glBufferData = (GL_BufferData)SDL_GL_GetProcAddress("glBufferData");
	glBufferSubData = (GL_BufferSubData)SDL_GL_GetProcAddress("glBufferSubData");

	glUseProgram = (GL_UseProgram)SDL_GL_GetProcAddress("glUseProgram");
	glCreateProgram = (GL_CreateProgram)SDL_GL_GetProcAddress("glCreateProgram");
	glDeleteProgram = (GL_DeleteProgram)SDL_GL_GetProcAddress("glDeleteProgram");
	glLinkProgram = (GL_LinkProgram)SDL_GL_GetProcAddress("glLinkProgram");
	glGetProgramiv = (GL_GetProgramiv)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (GL_GetProgramInfoLog)SDL_GL_GetProcAddress("glGetProgramInfoLog");

	glCreateShader = (GL_CreateShader)SDL_GL_GetProcAddress("glCreateShader");
	glDeleteShader = (GL_DeleteShader)SDL_GL_GetProcAddress("glDeleteShader");
	glShaderSource = (GL_ShaderSource)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (GL_CompileShader)SDL_GL_GetProcAddress("glCompileShader");
	glIsShader = (GL_IsShader)SDL_GL_GetProcAddress("glIsShader");
	glGetShaderiv = (GL_GetShaderiv)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (GL_GetShaderInfoLog)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glAttachShader = (GL_AttachShader)SDL_GL_GetProcAddress("glAttachShader");
	glDetachShader = (GL_DetachShader)SDL_GL_GetProcAddress("glDetachShader");

	glGetAttribLocation = (GL_GetAttribLocation)SDL_GL_GetProcAddress("glGetAttribLocation");
	glGetUniformLocation = (GL_GetUniformLocation)SDL_GL_GetProcAddress("glGetUniformLocation");
	glUniform1i = (GL_Uniform1i)SDL_GL_GetProcAddress("glUniform1i");
	glUniform4f = (GL_Uniform4f)SDL_GL_GetProcAddress("glUniform4f");

	glEnableVertexAttribArray = (GL_EnableVertexAttribArray)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (GL_DisableVertexAttribArray)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (GL_VertexAttribPointer)SDL_GL_GetProcAddress("glVertexAttribPointer");

	printLog(0, "Extensions loaded successfully.", NULL);
}

void executeCommand(Level *level, char *command)
{
	int index = 0;
	int length = 0;

	// NOTE: Should commandArray be static?
	char **commandArray = (char **)malloc(sizeof(char *));
	commandArray[index] = strtok(command, " ");

	while(commandArray[index] != NULL)
	{
		index++;
		length++;
		char **temp = (char **)realloc(commandArray, (index + 1) * sizeof(char *));

		if(temp != NULL)
		{
			commandArray = temp;
		}

		commandArray[index] = strtok(NULL, " ");
	}

	if(commandArray[0] != NULL)
	{
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
		else if(strcmp(commandArray[0], "scale") == 0)
		{
			// TODO: Remove this.
			if(length > 1)
			{
				// BLOCK_SIZE = atoi(commandArray[1]);
				// TODO: Reload the level with the new textures.
			}
		}
		else if(strcmp(commandArray[0], "debug") == 0)
		{
			DEBUG = !DEBUG;
		}
		else if(strcmp(commandArray[0], "vsync") == 0)
		{
			SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());
		}
		// TODO: Add disconnect option and free PACKET.
		else if(strcmp(commandArray[0], "host") == 0)
		{
			// TODO: Turn off multiplayer, check if the client is active.
			MULTIPLAYER = true;
			CLIENT = false;

			// TODO: Return on error.
			// TODO: Close the opened socket.
			if((SOCKET = SDLNet_UDP_Open(9999)) == NULL)
			{
				printLog(1, "Unable to open a socket on port ", "9999.");
				MULTIPLAYER = false;
			}

			if((PACKET = SDLNet_AllocPacket(512)) == NULL)
			{
				printLog(1, "Unable allocate a packet.", "");
				MULTIPLAYER = false;
			}

			// TODO: Add port support.
			if(SDLNet_ResolveHost(&ADDRESS, commandArray[1], 9999) == -1)
			{
				printLog(1, "Unable to resolve client ", commandArray[1]);
				printLog(1, "Error message: ", SDLNet_GetError());
			}
			else
			{
				printLog(0, "Server hosted on <IP_ADDRESS>:", "9999");
				// TODO: Get IP address from the client.
				SDLNet_UDP_Bind(SOCKET, 0, &ADDRESS);
			}
		}
		// TODO: Add disconnect option and free PACKET.
		else if(strcmp(commandArray[0], "connect") == 0)
		{
			if(length > 1)
			{
				// TODO: Check if host is active.
				MULTIPLAYER = true;
				CLIENT = true;

				if((SOCKET = SDLNet_UDP_Open(9999)) == NULL)
				{
					printLog(1, "Unable to open a socket.", "");
					MULTIPLAYER = false;
				}

				if(PACKET == NULL)
				{
					if((PACKET = SDLNet_AllocPacket(512)) == NULL)
					{
						printLog(1, "Unable allocate a packet.", "");
						MULTIPLAYER = false;
					}
				}

				// TODO: Add port support.
				if(SDLNet_ResolveHost(&ADDRESS, commandArray[1], 9999) == -1)
				{
					printLog(1, "Unable to resolve server ", commandArray[1]);
					printLog(1, "Error message: ", SDLNet_GetError());
				}
				else
				{
					printLog(0, "Connected to ", commandArray[1]);
					SDLNet_UDP_Bind(SOCKET, 0, &ADDRESS);
				}
			}
		}
	}

	free(commandArray);
}

void saveScreenshot()
{
	SDL_Surface *screenshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);

	unsigned char *pixels = (unsigned char *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
	glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	for(int i = 0; i < SCREEN_HEIGHT; i++)
	{
		memcpy((char *)screenshot->pixels + SCREEN_WIDTH * 3 * i, pixels + SCREEN_WIDTH * 3 * (SCREEN_HEIGHT - i - 1), SCREEN_WIDTH * 3);
	}

	free(pixels);

	// TODO: Add automatic numbering.
	IMG_SavePNG(screenshot, "screenshots/screenshot.png");
	SDL_FreeSurface(screenshot);

	// TODO: Error checking.
	printLog(0, "Screenshot saved successfully as", "\'screenshots/screenshot.png\'.");
}

void loadShader(GLuint *shaderProgram, const char *vsFilename, const char *fsFilename)
{
	printLog(0, "Loading vertex shader", vsFilename);
	printLog(0, "Loading fragment shader", fsFilename);

	FILE *vsFile = fopen(vsFilename, "r");
	FILE *fsFile = fopen(fsFilename, "r");

	if(vsFile == NULL)
	{
		printLog(1, "Unable to open vertex shader:", vsFilename);

		if(fsFile != NULL)
		{
			fclose(fsFile);
		}
	}
	else if(fsFile == NULL)
	{
		printLog(1, "Unable to open fragment shader:", fsFilename);
		fclose(vsFile);
	}
	else
	{
		// NOTE: Should fopen() have an argument "a", so there is no need to fseek()?
		fseek(vsFile, 0, SEEK_END);
		fseek(fsFile, 0, SEEK_END);

		long vsLength = ftell(vsFile);
		long fsLength = ftell(fsFile);

		fseek(vsFile, 0, SEEK_SET);
		fseek(fsFile, 0, SEEK_SET);

		GLchar **vsSource = (GLchar **)malloc(sizeof(GLchar *));
		vsSource[0] = (GLchar *)calloc(vsLength + 1, sizeof(GLchar));

		GLchar **fsSource = (GLchar **)malloc(sizeof(GLchar *));
		fsSource[0] = (GLchar *)calloc(fsLength + 1, sizeof(GLchar));

		fread(vsSource[0], sizeof(GLchar), vsLength, vsFile);
		fclose(vsFile);

		fread(fsSource[0], sizeof(GLchar), fsLength, fsFile);
		fclose(fsFile);

		GLint compileStatus = GL_TRUE;

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const GLchar **)vsSource, NULL);
		glCompileShader(vertexShader);
		free(vsSource);

		// TODO: Add a message on successful compilation.
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printLog(1, "Unable to compile vertex shader: ", vsFilename);
			printInfoLog(vertexShader);
			glDeleteShader(vertexShader);
		}

		printLog(0, "OK!", vsFilename);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, (const GLchar **)fsSource, NULL);
		glCompileShader(fragmentShader);
		free(fsSource);

		// TODO: Add a message on successful compilation.
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus == GL_FALSE)
		{
			printLog(1, "Unable to compile fragment shader: ", fsFilename);
			printInfoLog(fragmentShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
		}

		printLog(0, "OK!", fsFilename);

		glUseProgram(*shaderProgram);
		*shaderProgram = glCreateProgram();

		// NOTE: What to do if vertexShader is deleted?
		glAttachShader(*shaderProgram, vertexShader);
		glAttachShader(*shaderProgram, fragmentShader);

		glLinkProgram(*shaderProgram);

		GLint linkStatus;
		glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &linkStatus);

		if(linkStatus == GL_FALSE)
		{
			printInfoLog(*shaderProgram);
			glDeleteProgram(*shaderProgram);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		glDetachShader(*shaderProgram, vertexShader);
		glDetachShader(*shaderProgram, fragmentShader);
		glUseProgram(0);
	}
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

void drawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, uint32_t rgb_color, GLfloat alpha)
{
	glUseProgram(shaderProgram);
	glUniform4f(colorLocation, ((rgb_color >> 16) & 0xFF) / 255.0, ((rgb_color >> 8) & 0xFF) / 255.0, (rgb_color & 0xFF) / 255.0, alpha);

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

void drawEmptyRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat lineWidth, uint32_t rgb_color)
{
	glUseProgram(shaderProgram);
	glUniform4f(colorLocation, ((rgb_color >> 16) & 0xFF) / 255.0, ((rgb_color >> 8) & 0xFF) / 255.0, (rgb_color & 0xFF) / 255.0, 1.0f);

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
