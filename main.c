#include "globals.h"
#include "util.h"
#include "texture.h"
#include "level.h"
#include "player.h"
#include "font.h"
#include "animation.h"

// NOTE: The arguments are unused.
// TODO: Add proper error checking and reporting.
// TODO: Add error reporting with return values of functions (loadTexture()).
int main(int argc, char **argv)
{
	SDL_Window *window = NULL;
	initWindow(&window, "Zeno Engine");

	// NOTE: Should level be a global variable?
	Level level;
	initLevel(&level);
	loadLevel(&level, "level1");

	// TODO: Load assets from a file into array (e.g. array of textures) using IDs -> Engine.
	Font font2, font4;
	initFont(&font2);
	initFont(&font4);
	loadFont(&font2, "data/font2.dat");
	loadFont(&font4, "data/font4.dat");

	Player player;
	initPlayer(&player);
	loadPlayer(&player, "images/player.png");

	Texture interfaceTexture;
	initTexture(&interfaceTexture);
	loadTexture(&interfaceTexture, "images/interface.png");

	Texture background;
	initTexture(&background);
	loadTexture(&background, "images/background.png");

	loadShader(&shaderProgram, "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
	colorLocation = glGetUniformLocation(shaderProgram, "Color");

	loadShader(&texShader, "shaders/textureVertexShader.glsl", "shaders/textureFragmentShader.glsl");
	texPos = glGetAttribLocation(texShader, "TexturePosition");
	texCoords = glGetAttribLocation(texShader, "TexCoord");
	texColor = glGetUniformLocation(texShader, "TextureColor");

	SDL_Event event;

	bool quitGame = false;
	bool editor = false;
	bool console = false;

	char command[256] = {'\0'};
	int commandIndex = 0;
	uint8_t editorBlock = 0;

	int frames = 0, fps = 0;
	char engineInformation[256] = {'\0'};

	Uint32 fpsTimer = SDL_GetTicks();
	Uint32 currentTime = SDL_GetTicks();

	while(quitGame == false)
	{
		Uint32 previousTime = currentTime;
		currentTime = SDL_GetTicks();
		DELTA_TICKS = (currentTime - previousTime) / 1000.0f;

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
						quitGame = true;
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
									// TODO: Replace with SDL_StartTextInput()
									if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || event.key.keysym.sym == ' ')
									{
										command[commandIndex++] = event.key.keysym.sym;
										command[commandIndex] = '\0';
									}
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
			else if(console == true)
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_UP:
						break;

						case SDLK_DOWN:
						break;
					}
				}
			}
			else
			{
				handlePlayerEvent(&player, &event);
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
			updatePlayer(&player, &level);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);
		glTranslatef(-cameraX, -cameraY, 0.0f);

		//drawLevelVBO(&level);
		drawTexture(&background, 0.0f, 0.0f, NULL, 0.0f, 4.0f);
		drawPlayer(&player);

		glLoadIdentity();

		drawTexture(&interfaceTexture, 16.0f, SCREEN_HEIGHT - 16.0f - 64.0f, NULL, 0.0f, 1.0f);

		drawRectangle(20.0f, SCREEN_HEIGHT - 76.0f, player.Health, 23.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		drawEmptyRectangle(146.0f + (player.SelectedSkill - 1) * 32.0f + player.SelectedSkill * 2.0f, SCREEN_HEIGHT - 47.0f, 146.0f + (player.SelectedSkill - 1) * 32.0f + player.SelectedSkill * 2 + 32.0f, SCREEN_HEIGHT - 16.0f, 2.0f, 0.0f, 1.0f, 0.0f);

		sprintf(engineInformation, "%s (F1 - Log, F2 - Editor)\nFPS: %d", NAME_VERSION, fps);
		drawText(&font2, 7.0f, 5.0f, engineInformation, 0.0f, 0.0f, 0.0f);

		if(editor == true)
		{
			// TODO: Move console to the logger.
			if(console == true)
			{
				drawRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 15.0f - 15.0f, SCREEN_HEIGHT - 15.0f - (SCREEN_HEIGHT - 60.0f), 1.0f, 1.0f, 1.0f, 1.0f);
				drawEmptyRectangle(15.0f, SCREEN_HEIGHT - 60.0f, SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f, 2.0f, 0.0f, 0.0f, 0.0f);

				drawText(&font4, 22.0f, SCREEN_HEIGHT - 56.0f, "$ ", 0.0f, 0.0f, 0.0f);
				drawText(&font4, 55.0f, SCREEN_HEIGHT - 58.0f, command, 0.0f, 0.0f, 0.0f);

				// TODO: Command history.
			}
		}
		if(console == true)
		{
			drawRectangle(0.0f, 0.0f, SCREEN_WIDTH, 400.0f, 0.0f, 0.0f, 0.0f, 0.9f);

			for(int i = logIndex - 1; i >= 0 && 25.0f * i >= 0.0f; i--)
			{
				drawText(&font2, 7.0f, 22.0f * i, logs[i], 1.0f, 0.0f, 1.0f);
			}
		}

		SDL_GL_SwapWindow(window);

		frames++;

		if(SDL_GetTicks() - fpsTimer >= 1000)
		{
			fps = frames;
			frames = 0;
			fpsTimer = SDL_GetTicks();
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
