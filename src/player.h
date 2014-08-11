#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

// TODO: Alphabetically sort includes.
#include "globals.h"
// NOTE: Only for debugging.
#include "util.h"
#include "texture.h"
#include "level.h"
#include "animation.h"

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8

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

	uint32_t IdleTimer;
	bool IsIdle;

	// TODO: Move states to a boolean array.
	bool IsAttacking;

	Texture *InputTextures;
	uint8_t MaxInputLength;
	int8_t *InputList;
	uint8_t InputIndex;
	uint32_t InputTicks;
	uint8_t Input;
	uint8_t KeyState;
	// NOTE: Can combine with IsFacingLeft.
	uint8_t LastDirection;

	char InputString[32];
	uint8_t InputStringIndex;
	uint32_t InputStringTime;

	// TODO: Store control scheme in an array.
	// TODO: Add enumerators for keys and combos/inputs.
} Player;

void initPlayer(Player *player);
void loadPlayer(Player *player, const char *playerTexture);
void handlePlayerInput(Player *player, SDL_Event *event);
void updatePlayer(Player *player, Level *level);
void drawPlayer(Player *player);
void drawPlayerInput(Player *player);

#endif
