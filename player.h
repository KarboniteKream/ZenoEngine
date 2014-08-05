#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "globals.h"
// NOTE: Only for debugging.
#include "util.h"
#include "texture.h"
#include "level.h"
#include "animation.h"

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
	// NOTE: Better name?
	bool IsFacingLeft;

	Uint32 IdleTimer;
	bool IsIdle;

	bool IsAttacking;

	Texture *ComboTextures;
	uint8_t MaxComboLength;
	int8_t *Combo;
	uint8_t ComboIndex;
	char ComboString[32];
	uint8_t ComboStringIndex;
	uint32_t ComboTime;

	// TODO: Store control scheme in an array.
	// TODO: Add enumerators.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player, const char *playerTexture);
void handlePlayerEvent(Player *player, SDL_Event *event);
void updatePlayer(Player *player, Level *level);
void drawPlayer(Player *player);
void drawCombo(Player *player);

#endif
