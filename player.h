#ifndef PLAYER_H
#define PLAYER_H

// #include <math.h>

#include "globals.h"
#include "texture.h"
#include "level.h"

typedef struct
{
	Texture texture;

	int max_health;
	int health;
	uint8_t selected_skill;

	GLfloat x;
	GLfloat y;
	Rectangle bounding_box;

	GLfloat speed;
	GLfloat angle;

	// TODO: Implement aiming and shooting.
} Player;

void init_player(Player *player);
void load_player(Player *player);
void draw_player(Player *player);
void move_player(Level *level, Player *player);

#endif
