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
	int SelectedSkill;

	GLfloat X;
	GLfloat Y;
	GLfloat Angle;
	RectangleF BoundingBox;
	GLfloat Speed;

	// TODO: Implement aiming and shooting.
} Player;

void loadPlayer(Player *player);
void drawPlayer(Player *player);
void movePlayer(Level *level, Player *player);

#endif
