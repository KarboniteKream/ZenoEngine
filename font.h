#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#include "globals.h"
#include "texture.h"

#define FONT_NORMAL 0
#define FONT_LARGE 1

// NOTE: Should this be an array of structs?
typedef struct
{
	uint8_t Num;

	Texture *Textures;
	RectangleF **Clips;
	uint8_t *Height;
} Font;

void initFont(Font *font, uint8_t num);
void loadFont(Font *font, uint8_t index, const char* filename, GLuint shaderProgram);
void drawText(Font *font, uint8_t index, GLfloat x, GLfloat y, const char* text, uint32_t rgb_color);

#endif
