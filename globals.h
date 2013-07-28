#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// NOTE: Should this rather be read from a file?
#define NAME_VERSION "Zeno Engine 0.2.7c"

#define CAMERA_SPEED 600.0f
#define EDITOR_EDGE 20.0f

#define COLLIDABLE 0

extern float DELTA_TICKS;

extern GLuint BOUND_TEXTURE;
extern uint8_t BLOCK_SIZE;

extern GLfloat camera_x, camera_y;
// NOTE: Should mouse_x and mouse_y be globals?
extern GLfloat mouse_x, mouse_y;

typedef struct
{
	GLfloat X;
	GLfloat Y;
	GLfloat W;
	GLfloat H;
} RectangleF;

#endif
