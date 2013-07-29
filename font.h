#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#include "globals.h"
#include "texture.h"

// NOTE: Should this be a linked list?
typedef struct
{
	Texture texture;
	Rectangle *clips;
} Font;

void init_font(Font *font);
void load_font(Font *font, const char* filename);
void draw_text(Font *font, GLfloat x, GLfloat y, const char* text);

#endif
