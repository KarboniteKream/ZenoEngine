#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "texture.h"

// TODO: Add namespace.
#define LAYOUT 0
#define PROPERTIES 1
#define DEFAULT 0
#define COLLIDABLE 1
#define SLOPE_LEFT 2
#define SLOPE_RIGHT 3

typedef struct
{
	int Width;
	int Height;

	uint8_t ***Data;

	Texture LevelTexture;
	RectangleF *TexClips;
	VertexData *Vertices;
} Level;

void initLevel(Level *level);
void loadLevel(Level *level, const char *filename);
void saveLevel(Level *level, const char *filename);
void drawLevel(Level *level, bool useVBO);
void setLevelLayout(Level *level, int x, int y, int block);

#endif
