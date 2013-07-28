#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "texture.h"

typedef struct
{
	Texture LevelTexture;
	RectangleF *TextureClips;

	int Width;
	int Height;

	int **Layout;
	// TODO: Smaller variable sizes (uint8_t).
	int ***Properties;
	bool Debug;
} Level;

void initLevel(Level *level);
void loadLevel(Level *level, const char *filename);
void saveLevel(Level *level, const char *filename);
void drawLevel(Level *level);

#endif
