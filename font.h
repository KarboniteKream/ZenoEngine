#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#include "globals.h"
#include "texture.h"

#define FONT_NORMAL 0
#define FONT_LARGE 1

// TODO: Combine all fonts.
// NOTE: Should this be a linked list?
typedef struct
{
	uint8_t Num;

	Texture *Textures;
	RectangleF **Clips;
	uint8_t *Height;
} Font;

void initFont(Font *font, uint8_t num);
void loadFont(Font *font, uint8_t index, const char* filename);
void drawText(Font *font, uint8_t index, GLfloat x, GLfloat y, const char* text, GLfloat r, GLfloat g, GLfloat b);

#endif
