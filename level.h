#ifndef LEVEL_H
#define LEVEL_H

#define GL_GLEXT_PROTOTYPES

#include "globals.h"
#include "texture.h"

typedef struct
{
	Texture LevelTexture;
	Rectangle *TexClips;

	int Width;
	int Height;

	uint8_t **Layout;
	uint8_t ***Properties;

	bool Debug;
} Level;

void initLevel(Level *level);
void loadLevel(Level *level, const char *filename);
void saveLevel(Level *level, const char *filename);
void generateLevelVBO(Level *level);
void drawLevel(Level *level);
void drawLevelVBO(Level *level);

#endif
