#include <time.h>
#include <unistd.h>

#include "globals.h"
#include "util.h"
#include "texture.h"
#include "level.h"
#include "player.h"
#include "font.h"
#include "animation.h"
#include "particle.h"
#include "entity.h"

// TODO: Add proper error checking and reporting.
// TODO: Add error reporting with return values of functions (loadTexture()).
// TODO: Automatically call init*() with load*().
// TODO: Use stack instead of heap where applicable.
// TODO: Improve scale handling in calculations.
// TODO: Use smaller data types where possible.
int main(int argc, char **argv)
{
	// TEMP: Supress a warning.
	if(argc > 1)
	{
		printLog(0, "Arguments:", argv[1]);
	}

	SDL_Window *window = NULL;
	initWindow(&window, "Zeno Engine");

	loadShader(&shaderProgram, "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
	colorLocation = glGetUniformLocation(shaderProgram, "Color");

	loadShader(&texShader, "shaders/textureVertexShader.glsl", "shaders/textureFragmentShader.glsl");
	texPos = glGetAttribLocation(texShader, "TexturePosition");
	texCoords = glGetAttribLocation(texShader, "TexCoord");
	texColor = glGetUniformLocation(texShader, "TextureColor");

	// NOTE: Should level be a global variable?
	Level level;
	initLevel(&level);
	loadLevel(&level, "level1");

	Player player;
	initPlayer(&player);
	loadPlayer(&player, "images/player.png");

	Entity practiceTarget;
	initEntity(&practiceTarget);
	loadEntity(&practiceTarget, "images/scarecrow.png", 1100.0f, 256.0f);

	Texture background, interfaceTexture;
	initTexture(&background);
	initTexture(&interfaceTexture);
	loadTexture(&background, "images/background.png", 0);
	loadTexture(&interfaceTexture, "images/interface.png", 0);

	// TODO: Load assets from a file into array (e.g. array of textures) using IDs -> Engine.
	Font fonts;
	initFont(&fonts, 2);
	loadFont(&fonts, FONT_NORMAL, "data/font2.dat", texShader);
	loadFont(&fonts, FONT_LARGE, "data/font4.dat", texShader);

	ParticleSystem pSys;
	initParticleSystem(&pSys, level.Width * BLOCK_SIZE - 25.0f, level.Height * BLOCK_SIZE - 400.0f - BLOCK_SIZE - 25.0f, 400.0f, 250);
	setParticleTTL(&pSys, 0);
	srand(time(NULL));

	Player *players = NULL;
	bool playerInit = false;

	SDL_Event event;

	bool quitGame = false;
	bool editor = false;
	bool console = false;

	char command[256] = {'\0'};
	int commandIndex = 0;
	uint8_t editorBlock = 0;

	int frames = 0, fps = 0;
	char engineInformation[256] = {'\0'};

	uint32_t fpsTimer = SDL_GetTicks();
	uint32_t packetTimer = SDL_GetTicks();
	uint32_t currentTime = SDL_GetTicks();

	while(quitGame == false)
	{
		uint32_t previousTime = currentTime;
		currentTime = SDL_GetTicks();
		DELTA_TICKS = (currentTime - previousTime) / 1000.0f;

		// NOTE: This is used for multiplayer.
		// NOTE: The game will not run properly below 30 FPS.
		if(DELTA_TICKS > 0.034f)
		{
			DELTA_TICKS = 0.034f;
		}

		// FIXME: Execute only on first connect and a new player joining.
		if(players == NULL && MULTIPLAYER == true)
		{
			// TODO: Free players.
			players = (Player *)malloc(1 * sizeof(Player));
		}

		if(MULTIPLAYER == true)
		{
			if(playerInit == false)
			{
				initPlayer(&players[0]);
				loadPlayer(&players[0], "images/player.png");
				playerInit = true;
			}

			while(SDLNet_UDP_Recv(SOCKET, PACKET) == 1)
			{
				/*
				// NOTE: Is there a faster way?
				char temp[PACKET->len];
				strcpy(temp, (char *)PACKET->data);

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

				if(strcmp(commandArray[0], "init") == 0)
				{
					initPlayer(&players[0]);
					loadPlayer(&players[0], "images/player.png");
					playerInit = true;
				}
				else
				{
					players[0].X = atof(commandArray[0]);
					players[0].Y = atof(commandArray[1]);
				}

				free(commandArray);
				*/

				// NOTE: Is it necessary to convert to char *?
				sscanf((char *)PACKET->data, "%5f %5f", &players[0].X, &players[0].Y);
			}
		}

		while(SDL_PollEvent(&event) != 0)
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
						if(console == false && editor == false)
						{
							quitGame = true;
						}
					break;

					case SDLK_F1:
						console = !console;
					break;

					case SDLK_F2:
						editor = !editor;
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

			// TODO: Rewrite and combine with the if statement above.
			if(editor == true)
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_RETURN:
							console = true;
						break;

						case SDLK_1: case SDLK_2:
						case SDLK_3: case SDLK_4:
						case SDLK_5: case SDLK_6:
						case SDLK_7:
							editorBlock = event.key.keysym.sym - 48 - 1;
						break;

						case SDLK_ESCAPE:
							editor = false;
						break;

						default:
						break;
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
			else if(console == true)
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_RETURN:
							executeCommand(&level, command);
							commandIndex = 0;
							command[0] = '\0';
						break;

						case SDLK_BACKSPACE:
							if(commandIndex > 0)
							{
								command[--commandIndex] = '\0';
							}
						break;

						case SDLK_UP:
						break;

						case SDLK_DOWN:
						break;

						case SDLK_ESCAPE:
							console = false;
						break;

						default:
							if(commandIndex < 255)
							{
								// TODO: Replace with SDL_StartTextInput()
								if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9') || event.key.keysym.sym == ' ' || event.key.keysym.sym == '.')
								{
									command[commandIndex++] = event.key.keysym.sym;
									command[commandIndex] = '\0';
								}
							}
						break;
					}
				}
			}
			else
			{
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						startEntity(&practiceTarget);
					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						char pos[32];
						snprintf(pos, 32, "%.0f, %.0f", mouseX, mouseY);
						printLog(2, pos, NULL);
					}
				}

				handlePlayerEvent(&player, &event);
			}
		}

		if(editor == true)
		{
			if(mouseX <= EDITOR_EDGE)
			{
				cameraX -= CAMERA_SPEED * DELTA_TICKS;
			}
			else if(mouseX > (SCREEN_WIDTH - EDITOR_EDGE))
			{
				cameraX += CAMERA_SPEED * DELTA_TICKS;
			}

			if(mouseY <= EDITOR_EDGE)
			{
				cameraY -= CAMERA_SPEED * DELTA_TICKS;
			}
			else if(mouseY > (SCREEN_HEIGHT - EDITOR_EDGE))
			{
				cameraY += CAMERA_SPEED * DELTA_TICKS;
			}
		}
		else
		{
			updatePlayer(&player, &level);
			updateEntity(&practiceTarget);
		}

		if(cameraX < 0.0f)
		{
			cameraX = 0.0f;
		}
		else if(cameraX > level.Width * BLOCK_SIZE - SCREEN_WIDTH)
		{
			cameraX = level.Width * BLOCK_SIZE - SCREEN_WIDTH;
		}

		if(cameraY < 0.0f)
		{
			cameraY = 0.0f;
		}
		else if(cameraY > level.Height * BLOCK_SIZE - SCREEN_HEIGHT)
		{
			cameraY = level.Height * BLOCK_SIZE - SCREEN_HEIGHT;
		}

		uint64_t renderTime = SDL_GetPerformanceCounter();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);
		glTranslatef(-cameraX, -cameraY, 0.0f);

		drawLevel(&level, true);
		//drawTexture(&background, 0.0f, 0.0f, NULL, 0.0f, 4.0f, false);
		debugParticleSystem(&pSys);
		updateParticleSystem(&pSys);

		if(MULTIPLAYER == true && playerInit == true)
		{
			drawPlayer(&players[0]);
		}

		drawEntity(&practiceTarget);
		drawPlayer(&player);

		glLoadIdentity();

		drawTexture(&interfaceTexture, 16.0f, SCREEN_HEIGHT - 16.0f - 64.0f, NULL, 0.0f, 1.0f, false);

		drawRectangle(20.0f, SCREEN_HEIGHT - 76.0f, player.Health, 23.0f, 0xFF0000, 1.0f);
		drawEmptyRectangle(146.0f + (player.SelectedSkill - 1) * 32.0f + player.SelectedSkill * 2.0f, SCREEN_HEIGHT - 47.0f, 32.0f, 31.0f, 2.0f, 0x00FF00);

		// TODO: Check if a rebuild is necessary.
		if(editor == true)
		{
			snprintf(engineInformation, 256, "%s (EDITOR)", NAME_VERSION);
		}
		else
		{
			snprintf(engineInformation, 256, "%s (F1 - Log, F2 - Editor, F12 - Screenshot)", NAME_VERSION);
		}

		drawText(&fonts, FONT_NORMAL, 7.0f, 5.0f, engineInformation, 0x000000);

		char combo[player.MaxComboLength + 1];
		for(int i = 0; i < player.MaxComboLength; i++)
		{
			combo[i] = player.Combo[i] == -1 ? 'X' : player.Combo[i] + 48;
		}
		combo[player.MaxComboLength] = '\0';
		drawText(&fonts, FONT_LARGE, 100.0f, 60.0f, combo, 0x000000);
		drawText(&fonts, FONT_LARGE, 100.0f, 110.0f, player.ComboString, 0x000000);
		drawCombo(&player);

		// FIXME: Timer doesn't work properly on all platforms?
		char renderTimeString[32];
		renderTime = SDL_GetPerformanceCounter() - renderTime;
		snprintf(renderTimeString, 32, "FPS: %d (%.0f us)", fps, (float)(renderTime) / SDL_GetPerformanceFrequency() * 1000000.0f);
		drawText(&fonts, FONT_NORMAL, 7.0f, 27.0f, renderTimeString, 0x000000);

		if(console == true)
		{
			drawRectangle(0.0f, 0.0f, SCREEN_WIDTH, 400.0f, 0x000000, 0.9f);

			for(int i = logIndex - 1; i >= 0 && 25.0f * i >= 0.0f; i--)
			{
				drawText(&fonts, FONT_NORMAL, 7.0f, 22.0f * i, logs[i], 0xFF00FF);
			}

			drawRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 30.0f, 45.0f, 0xFFFFFF, 1.0f);
			drawEmptyRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 30.0f, 45.0f, 2.0f, 0x000000);

			drawText(&fonts, FONT_LARGE, 22.0f, SCREEN_HEIGHT - 56.0f, "$ ", 0x000000);
			drawText(&fonts, FONT_LARGE, 55.0f, SCREEN_HEIGHT - 58.0f, command, 0x000000);

			// TODO: Command history.
		}

		SDL_GL_SwapWindow(window);

		frames++;

		// TODO: Implement an optional frame limiter. [Final Build]
		// SDL_Delay(16);

		if(SDL_GetTicks() - fpsTimer >= 1000)
		{
			fps = frames;
			frames = 0;
			fpsTimer = SDL_GetTicks();
		}

		if(MULTIPLAYER == true && SDL_GetTicks() - packetTimer >= 10)
		{
			snprintf((char *)PACKET->data, PACKET->maxlen, "%.0f %.0f", player.X, player.Y);
			PACKET->len = strlen((char *)PACKET->data) + 1;
			SDLNet_UDP_Send(SOCKET, 0, PACKET);
			packetTimer = SDL_GetTicks();
		}
	}

	SDLNet_FreePacket(PACKET);
	SDL_DestroyWindow(window);
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}
