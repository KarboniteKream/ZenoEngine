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

	bool IsMoving;
	bool IsStopping;
	GLfloat Speed;
	GLfloat CurrentSpeed;

	bool IsJumping;
	GLfloat JumpSpeed;
	GLfloat CurrentJumpSpeed;

	bool *KeyStates;
	// TODO: Store control scheme in an array.
	// TODO: Add enumerators.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player, const char *dataFile);
void drawPlayer(Player *player);
void handlePlayerEvent(Player *player, SDL_Event *event);
void updatePlayer(Player *player, Level *level);

#endif
