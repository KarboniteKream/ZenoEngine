#include "globals.h"
#include "texture.h"
#include "level.h"
#include "player.h"
#include "font.h"

void save_screenshot();
void execute_command(Level *level, const char *command);

// NOTE: The arguments are unused.
int main(int argc, char **argv)
{
	// TODO: Add SDL_GetError() and glGetError() support.
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		fprintf(stderr, "An error has occurred while initializing SDL.\n");
		exit(1);
	}

	// TODO: Support for borderless window.
	// TODO: Fullscreen support.
	SDL_Window *window = SDL_CreateWindow(NAME_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == NULL)
	{
		fprintf(stderr, "An error has occurred while creating the SDL window.\n");
		exit(1);
	}

	SDL_SetWindowIcon(window, SDL_LoadBMP("resources/icon.bmp"));
	SDL_GLContext context = SDL_GL_CreateContext(window);

	if(context == NULL)
	{
		fprintf(stderr, "An error has occurred while creating the OpenGL context.\n");
		exit(1);
	}

	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_ShowCursor(SDL_DISABLE);

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

	atexit(SDL_Quit);

	SDL_Event event;

	bool quit_game = false;
	bool editor = false;
	bool console = false;

	char command[256] = {0};
	int command_index = 0;
	int editor_block = 0;

	// NOTE: Should level be a global variable?
	Level level;
	initLevel(&level);
	loadLevel(&level, "level1");

	// TODO: Load assets from a file into array (e.g. array of textures) using IDs -> Engine.
	Font *fonts = malloc(2 * sizeof(Font));
	initFont(&fonts[0]);
	initFont(&fonts[1]);
	loadFont(&fonts[0], "data/font2.dat");
	loadFont(&fonts[1], "data/font4.dat");

	Player player;
	loadPlayer(&player);

	Texture cursor_texture, minimap_texture, interface_texture, pistol_texture;
	initTexture(&minimap_texture);
	initTexture(&interface_texture);
	initTexture(&cursor_texture);
	initTexture(&pistol_texture);
	loadTexture(&minimap_texture, "images/minimap.png");
	loadTexture(&interface_texture, "images/interface.png");
	loadTexture(&cursor_texture, "images/cursor.png");
	loadTexture(&pistol_texture, "images/pistolSilencer.png");

	int current_time = SDL_GetTicks();

	while(quit_game == false)
	{
		int previous_time = current_time;
		current_time = SDL_GetTicks();
		DELTA_TICKS = (current_time - previous_time) / 1000.0f;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit_game = true;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit_game = true;
						break;

					case SDLK_F1:
						editor = !editor;
						// TODO: Exiting out of the editor should close the console.
						SDL_ShowCursor(!SDL_ShowCursor(-1));
						break;

					case SDLK_F12:
						save_screenshot();
						break;
				}
			}
			else if(event.type == SDL_MOUSEMOTION)
			{
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
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
								execute_command(&level, command);
								// FIXME: Don't close the console, unless the command string is empty.
								command_index = command[0] = 0;
								console = false;
								break;

							case SDLK_BACKSPACE:
								if(command_index > 0)
								{
									command[--command_index] = 0;
								}
								break;

							default:
								command[command_index++] = event.key.keysym.sym;
								command[command_index] = 0;
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
								editor_block = event.key.keysym.sym - 48 - 1;
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
						level.Layout[(int)((mouse_x + camera_x) / BLOCK_SIZE)][(int)((mouse_y + camera_y) / BLOCK_SIZE)] = editor_block;
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
			if(mouse_x < EDITOR_EDGE)
			{
				camera_x -= CAMERA_SPEED * DELTA_TICKS;

				if(camera_x < 0.0f)
				{
					camera_x = 0.0f;
				}
			}
			if(mouse_x >= (SCREEN_WIDTH - EDITOR_EDGE))
			{
				camera_x += CAMERA_SPEED * DELTA_TICKS;

				if(camera_x > (level.Width * BLOCK_SIZE - SCREEN_WIDTH))
				{
					camera_x = (level.Width * BLOCK_SIZE - SCREEN_WIDTH);
				}
			}
			if(mouse_y < EDITOR_EDGE)
			{
				camera_y -= CAMERA_SPEED * DELTA_TICKS;

				if(camera_y < 0.0f)
				{
					camera_y = 0.0f;
				}
			}
			if(mouse_y >= (SCREEN_HEIGHT - EDITOR_EDGE))
			{
				camera_y += CAMERA_SPEED * DELTA_TICKS;

				if(camera_y > (level.Height * BLOCK_SIZE - SCREEN_HEIGHT))
				{
					camera_y = (level.Height * BLOCK_SIZE - SCREEN_HEIGHT);
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
		glTranslatef(-camera_x, -camera_y, 0.0f);

		drawLevel(&level);
		drawPlayer(&player);

		glLoadIdentity();

		drawTexture(&minimap_texture, SCREEN_WIDTH - 250.0f - 10.0f, 10.0f, NULL, 0.0f);
		drawTexture(&interface_texture, 16.0f, SCREEN_HEIGHT - 16.0f - 64.0f, NULL, 0.0f);
		drawTexture(&pistol_texture, 16.0f + 64.0f + 2.0f, SCREEN_HEIGHT - 16.0f - 64.0f, NULL, 0.0f);

		// TODO: Replace immediate mode.
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(16.0f + 4.0f, SCREEN_HEIGHT - 64.0f - 16.0f + 4.0f);
			glVertex2f(16.0f + 4.0f + player.Health, SCREEN_HEIGHT - 64.0f - 16.0f + 4.0f);
			glVertex2f(16.0f + 4.0f + player.Health, SCREEN_HEIGHT - 16.0f - 33.0f - 4.0f);
			glVertex2f(16.0f + 4.0f, SCREEN_HEIGHT - 16.0f - 33.0f - 4.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
		glEnd();

		glLineWidth(2.0f);
		glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f, SCREEN_HEIGHT - 16.0f - 32.0f + 1.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 32.0f, SCREEN_HEIGHT - 16.0f - 32.0f + 1.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 32.0f - 1.0f, SCREEN_HEIGHT - 16.0f - 32.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 32.0f - 1.0f, SCREEN_HEIGHT - 16.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 32.0f - 1.0f, SCREEN_HEIGHT - 16.0f - 1.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f, SCREEN_HEIGHT - 16.0f - 1.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 1.0f, SCREEN_HEIGHT - 16.0f - 1.0f);
			glVertex2f(16.0f + 64.0f + 2.0f + 64.0f + (player.SelectedSkill - 1.0f) * 32.0f + player.SelectedSkill * 2.0f + 1.0f, SCREEN_HEIGHT - 16.0f - 32.0f + 1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
		glEnd();
		glLineWidth(1.0f);

		drawText(&fonts[0], 7.0f, 5.0f, NAME_VERSION);

		if(editor == true)
		{
			drawText(&fonts[0], 171.0f, 5.0f, " - EDITOR");

			if(console == true)
			{
				glBegin(GL_QUADS);
					glVertex2f(15.0f, SCREEN_HEIGHT - 60.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 60.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f);
					glVertex2f(15.0f, SCREEN_HEIGHT - 15.0f);
				glEnd();

				glLineWidth(2.0f);
				glBegin(GL_LINES);
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2f(15.0f, SCREEN_HEIGHT - 60.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 60.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 60.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f);
					glVertex2f(SCREEN_WIDTH - 15.0f, SCREEN_HEIGHT - 15.0f);
					glVertex2f(15.0f, SCREEN_HEIGHT - 15.0f);
					glVertex2f(15.0f, SCREEN_HEIGHT - 15.0f);
					glVertex2f(15.0f, SCREEN_HEIGHT - 60.0f);
					glColor3f(1.0f, 1.0f, 1.0f);
				glEnd();
				glLineWidth(1.0f);

				drawText(&fonts[1], 22.0f, SCREEN_HEIGHT - 56.0f, "$ ");
				drawText(&fonts[1], 55.0f, SCREEN_HEIGHT - 58.0f, command);

				// TODO: Command history.
			}
		}
		else
		{
			drawText(&fonts[0], 171.0f, 5.0f, " - Press F1 for Editor");
			drawTexture(&cursor_texture, mouse_x - 8.0f, mouse_y - 8.0f, NULL, 0.0f);
		}


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

void save_screenshot()
{
	unsigned char *pixels = (unsigned char *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
	unsigned char *screenshot = (unsigned char *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);

	glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	for(int i = 0; i < SCREEN_HEIGHT; i++)
    {
        memcpy(screenshot + SCREEN_WIDTH * 4 * i, pixels + SCREEN_WIDTH * 4 * (SCREEN_HEIGHT - i - 1), SCREEN_WIDTH * 4);
    }

    // TODO: Add automatic numbering.
	lodepng_encode32_file("screenshots/screenshot.png", screenshot, SCREEN_WIDTH, SCREEN_HEIGHT);

	free(pixels);
	free(screenshot);
}

void execute_command(Level *level, const char *command)
{
	if(strlen(command) > 0)
	{
		char temp[strlen(command) + 1];
		strcpy(temp, command);

		char **command_array = (char **)malloc(sizeof(char *));
		int index = 0, length = 0;
		command_array[index] = strtok(temp, " ");

		while(command_array[index] != NULL)
		{
			index++;
			length++;
			command_array = (char **)realloc(command_array, (index + 1) * sizeof(char *));
			command_array[index] = strtok(NULL, " ");
		}

		if(strcmp(command_array[0], "save") == 0)
		{
			if(length > 1)
			{
				if(length > 2 && strcmp(command_array[1], "level") == 0)
				{
					saveLevel(level, command_array[2]);
				}
			}
		}
		else if(strcmp(command_array[0], "load") == 0)
		{
			if(length > 1)
			{
				if(strcmp(command_array[1], "level") == 0)
				{
					loadLevel(level, command_array[2]);
				}
			}
		}
		else if(strcmp(command_array[0], "set") == 0)
		{
			if(length > 1)
			{
				// TODO: This should be removed some day.
				if(strcmp(command_array[1], "scale") == 0)
				{
					BLOCK_SIZE = atoi(command_array[2]);
					// TODO: Reload the level with the new textures.
				}
			}
		}
		else if(strcmp(command_array[0], "debug") == 0)
		{
			if(length > 1)
			{
				level->Debug = !level->Debug;
			}
		}
	}
}
