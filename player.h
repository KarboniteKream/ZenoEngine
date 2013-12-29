#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "globals.h"
#include "texture.h"
#include "level.h"
#include "animation.h"
// NOTE: Only for debugging.
#include "util.h"

typedef struct
{
	Texture PlayerTexture;
	Animation *Animations;
	Animation *Attacks;

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
	bool IsFacingLeft;

	Uint32 IdleTimer;
	bool IsIdle;

	bool IsAttacking;

	// TODO: Store control scheme in an array.
	// TODO: Add enumerators.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player, const char *playerTexture);
void drawPlayer(Player *player);
void handlePlayerEvent(Player *player, SDL_Event *event);
void updatePlayer(Player *player, Level *level);

#endif
