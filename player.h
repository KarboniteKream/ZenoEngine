#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "texture.h"
#include "level.h"
#include "animation.h"

#include <math.h>

typedef struct
{
	Texture PlayerTexture;
	Animation *Animations;

	int MaxHealth;
	int Health;
	uint8_t SelectedSkill;

	GLfloat X;
	GLfloat Y;
	GLfloat Scale;

	RectangleF BoundingBox;

	GLfloat Speed;
	GLfloat JumpSpeed;

	bool IsJumping;
	bool IsMoving;

	// TODO: Implement aiming and shooting.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player, const char *dataFile);
void drawPlayer(Player *player);
void movePlayer(Level *level, Player *player);

#endif
