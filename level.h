#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "texture.h"

typedef struct
{
	Texture texture;
	Rectangle *texture_clips;

	int width;
	int height;

	uint8_t **layout;
	uint8_t ***properties;

	bool debug;
} Level;

void init_level(Level *level);
void load_level(Level *level, const char *filename);
void save_level(Level *level, const char *filename);
void draw_level(Level *level);

#endif
