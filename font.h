#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#include "globals.h"
#include "texture.h"

// NOTE: Should this be a linked list?
typedef struct
{
	Texture FontTexture;
	Rectangle *Clips;
} Font;

void initFont(Font *font);
void loadFont(Font *font, const char* filename);
void drawText(Font *font, GLfloat x, GLfloat y, const char* text);

#endif
