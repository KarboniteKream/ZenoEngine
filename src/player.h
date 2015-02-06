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
#define UP_LEFT 16
#define UP_RIGHT 32
#define DOWN_LEFT 64
#define DOWN_RIGHT 128
#define PUNCH 256

#define RUNNING 0
#define STOPPING 1
#define JUMPING 2
#define ATTACKING 3
#define IDLE 4

typedef struct
{
	Texture PlayerTexture;
	Animation *Animations;
	Animation *AttackAnimations;

	int MaxHealth;
	int Health;
	uint8_t SelectedSkill;

	GLfloat X;
	GLfloat Y;
	GLfloat Scale;

	RectangleF BoundingBox;

	bool State[5];

	GLfloat Speed;
	GLfloat CurrentSpeed;

	GLfloat JumpSpeed;
	GLfloat CurrentJumpSpeed;

	bool *KeyStates;
	// NOTE: Better name?
	bool IsFacingLeft;

	uint32_t IdleTimer;

	Texture *InputTextures;
	uint8_t MaxInputLength;
	int8_t *InputList;
	uint8_t InputIndex;
	uint32_t InputTicks;
	uint16_t Input;
	// TODO: Combine with KeyStates?
	uint16_t KeyState;
	// NOTE: Can combine with IsFacingLeft.
	uint8_t LastDirection;

	char InputString[32];
	uint8_t InputStringIndex;
	uint32_t InputStringTime;

	bool AttackInput;
	int8_t AttackID;
	uint16_t Attacks[2][4];

	int8_t PrevAnimation;
	int8_t CurrAnimation;
	int8_t NextAnimation;

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
