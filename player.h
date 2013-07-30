#ifndef PLAYER_H
#define PLAYER_H

// #include <math.h>

#include "globals.h"
#include "texture.h"
#include "level.h"

typedef struct
{
	Texture PlayerTexture;

	int MaxHealth;
	int Health;
	uint8_t SelectedSkill;

	GLfloat X;
	GLfloat Y;
	Rectangle BoundingBox;

	GLfloat Speed;
	GLfloat Angle;

	// TODO: Implement aiming and shooting.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player);
void drawPlayer(Player *player);
void movePlayer(Level *level, Player *player);

#endif
