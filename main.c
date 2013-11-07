#include "globals.h"
#include "util.h"
#include "texture.h"
#include "level.h"
#include "player.h"
#include "font.h"

void saveScreenshot();
void executeCommand(Level *level, const char *command);

// NOTE: The arguments are unused.
// TODO: Add error checking and reporting.
// TODO: Add error reporting with return values of functions (loadTexture()).
int main(int argc, char **argv)
{
	// TODO: Add SDL_GetError() and glGetError() support.
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		fprintf(stderr, "An error has occurred while initializing SDL.\n");
		exit(1);
	}

	atexit(SDL_Quit);

	// TODO: Support for borderless window.
	// TODO: Fullscreen support.
	SDL_Window *window = SDL_CreateWindow(NAME_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == NULL)
	{
		fprintf(stderr, "An error has occurred while creating the SDL window.\n");
		exit(1);
	}

	if(SDL_GL_CreateContext(window) == NULL)
	{
		fprintf(stderr, "An error has occurred while creating the OpenGL context.\n");
		exit(1);
	}

	loadExtensions();

	//SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetWindowIcon(window, SDL_LoadBMP("resources/icon.bmp"));
	SDL_ShowCursor(SDL_ENABLE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

	if(glGetError() != GL_NO_ERROR)
	{
		fprintf(stderr, "An error has occurred while initializing OpenGL.\n");
		exit(1);
	}

	// SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Tis' an error.", window);

	SDL_Event event;

	bool quitGame = false;
	bool editor = false;
	bool console = false;

	char command[256] = {'\0'};
	unsigned int commandIndex = 0;
	uint8_t editorBlock = 0;

	// NOTE: Should level be a global variable?
	Level level;
	initLevel(&level);
	loadLevel(&level, "level1");

	// TODO: Load assets from a file into array (e.g. array of textures) using IDs -> Engine.
	Font fonts[2];
	initFont(&fonts[0]);
	initFont(&fonts[1]);
	loadFont(&fonts[0], "data/font2.dat");
	loadFont(&fonts[1], "data/font4.dat");

	Player player;
	initPlayer(&player);
	loadPlayer(&player);

	Texture minimapTexture, interfaceTexture, pistolTexture;
	initTexture(&minimapTexture);
	initVBO(&minimapTexture, 4);
	initTexture(&interfaceTexture);
	initTexture(&pistolTexture);
	loadTexture(&minimapTexture, "images/minimap.png");
	loadTexture(&interfaceTexture, "images/interface.png");
	loadTexture(&pistolTexture, "images/pistolSilencer.png");

	Texture background, chPlayer;
	initTexture(&background);
	initTexture(&chPlayer);
	loadTexture(&background, "images/background.png");
	loadTexture(&chPlayer, "images/chPlayer.png");

	unsigned int startTime = SDL_GetTicks();
	unsigned int currentTime = SDL_GetTicks();

	int frames = 0;
	float fps = 0.0f;
	char engineInformation[256];

	loadShader(&shaderProgram, "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
	colorLocation = glGetUniformLocation(shaderProgram, "Color");

	while(quitGame == false)
	{
		unsigned int previousTime = currentTime;
		currentTime = SDL_GetTicks();
		DELTA_TICKS = (currentTime - previousTime) / 1000.0f;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quitGame = true;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quitGame = true;
						break;

					case SDLK_F1:
						editor = !editor;
						// TODO: Exiting out of the editor should close the console.
						//SDL_ShowCursor(!SDL_ShowCursor(-1));
						break;

					case SDLK_F12:
						saveScreenshot();
						break;
				}
			}
			else if(event.type == SDL_MOUSEMOTION)
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			}

			if(editor == true)
			{
				if(event.type == SDL_KEYDOWN)
				{
					if(console == true)
					{
						switch(event.key.keysym.sym)
						{
							case SDLK_RETURN:
								executeCommand(&level, command);
								// FIXME: Don't close the console, unless the command string is empty or the specified key is pressed.
								commandIndex = 0;
								command[0] = '\0';
								console = false;
								break;

							case SDLK_BACKSPACE:
								if(commandIndex > 0)
								{
									command[--commandIndex] = '\0';
								}
								break;

							default:
								// TODO: What about the Z and Y keys? Perhaps a setting in the options screen?
								if(commandIndex < 255)
								{
									command[commandIndex++] = event.key.keysym.sym;
									command[commandIndex] = '\0';
								}
								break;
						}
					}
					else
					{
						switch(event.key.keysym.sym)
						{
							case SDLK_RETURN:
								console = true;
								break;

							case SDLK_1: case SDLK_2:
							case SDLK_3: case SDLK_4:
							case SDLK_5:
								editorBlock = event.key.keysym.sym - 48 - 1;
								break;

							default:
								break;
						}
					}
				}
				else if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						// NOTE: Should I rebuild VBO only when exiting editor mode?
						setLevelLayout(&level, (int)((mouseX + cameraX) / BLOCK_SIZE), (int)((mouseY + cameraY) / BLOCK_SIZE), editorBlock);
					}
				}
			}
			else
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_j:
							// TODO: Move into a separate function?
							player.Health -= 4;
							if(player.Health < 0)
							{
								player.Health = 0;
							}
							break;

						case SDLK_k:
							player.Health += 4;
							if(player.Health > player.MaxHealth)
							{
								player.Health = player.MaxHealth;
							}
							break;

						case SDLK_1: case SDLK_2:
						case SDLK_3: case SDLK_4:
							player.SelectedSkill = event.key.keysym.sym - 48;
							break;

						default:
							break;
					}
				}
				else if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						// TODO: Player shoots.
					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						// TODO: Player starts aiming.
					}
				}
				else if(event.type == SDL_MOUSEBUTTONUP)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{

					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						// TODO: Player stops aiming.
					}
				}
			}
		}

		if(editor == true)
		{
			if(mouseX < EDITOR_EDGE)
			{
				cameraX -= CAMERA_SPEED * DELTA_TICKS;

				if(cameraX < 0.0f)
				{
					cameraX = 0.0f;
				}
			}
			if(mouseX >= (SCREEN_WIDTH - EDITOR_EDGE))
			{
				cameraX += CAMERA_SPEED * DELTA_TICKS;

				if(cameraX > (level.Width * BLOCK_SIZE - SCREEN_WIDTH))
				{
					cameraX = (level.Width * BLOCK_SIZE - SCREEN_WIDTH);
				}
			}
			if(mouseY < EDITOR_EDGE)
			{
				cameraY -= CAMERA_SPEED * DELTA_TICKS;

				if(cameraY < 0.0f)
				{
					cameraY = 0.0f;
				}
			}
			if(mouseY >= (SCREEN_HEIGHT - EDITOR_EDGE))
			{
				cameraY += CAMERA_SPEED * DELTA_TICKS;

				if(cameraY > (level.Height * BLOCK_SIZE - SCREEN_HEIGHT))
				{
					cameraY = (level.Height * BLOCK_SIZE - SCREEN_HEIGHT);
				}
			}
		}
		else
		{
			movePlayer(&level, &player);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);
		glTranslatef(-cameraX, -cameraY, 0.0f);

		//drawLevelVBO(&level);
		drawTexture(&background, 0.0f, 0.0f, NULL, 0.0f, 4.0f);
		//drawPlayer(&player);
		drawTexture(&chPlayer, 175.0f, 138.0f, NULL, 0.0f, 4.0f);

		// drawRectangle(10.0f, 50.0f, 100.0f, 150.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();

		//drawTextureVBO(&minimapTexture, SCREEN_WIDTH - minimapTexture.Width - 10.0f, 10.0f, NULL, 0.0f);
		drawTexture(&interfaceTexture, 16.0f, SCREEN_HEIGHT - 16.0f - 64.0f, NULL, 0.0f, 1.0f);
		//drawTexture(&pistolTexture, 82.0f, SCREEN_HEIGHT - 80.0f, NULL, 0.0f, 1.0f);

		drawRectangle(20.0f, SCREEN_HEIGHT - 76.0f, 20.0f + player.Health, SCREEN_HEIGHT - 53.0f, 1.0f, 0.0f, 0.0f);
		drawEmptyRectangle(146.0f + (player.SelectedSkill - 1) * 32.0f + player.SelectedSkill * 2.0f, SCREEN_HEIGHT - 47.0f, 146.0f + (player.SelectedSkill - 1) * 32.0f + player.SelectedSkill * 2 + 32.0f, SCREEN_HEIGHT - 16.0f, 2.0f, 0.0f, 1.0f, 0.0f);

		if(editor == true)
		{
			sprintf(engineInformation, "%s - EDITOR\nFPS: %.1f", NAME_VERSION, fps);

			if(console == true)
			{
				drawRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f, 1.0f, 1.0f, 1.0f);
				drawEmptyRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f, 2.0f, 0.0f, 0.0f, 0.0f);

				drawText(&fonts[1], 22.0f, SCREEN_HEIGHT - 56.0f, "$ ");
				drawText(&fonts[1], 55.0f, SCREEN_HEIGHT - 58.0f, command);

				// TODO: Command history.
			}
		}
		else
		{
			sprintf(engineInformation, "%s - Press F1 for Editor\nFPS: %.1f", NAME_VERSION, fps);
			//drawTexture(&cursorTexture, mouseX - 8.0f, mouseY - 8.0f, NULL, 0.0f, 1.0f);
		}

		drawText(&fonts[0], 7.0f, 5.0f, engineInformation);

		SDL_GL_SwapWindow(window);

		frames++;
		fps = (frames / (float)(SDL_GetTicks() - startTime)) * 1000;
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
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

		free(commandArray);
	}
}
