#include "player.h"

void init_player(Player *player)
{
	init_texture(&player->texture);

	player->max_health = 0;
	player->health = 0;
	player->selected_skill = 0;

	player->x = 0;
	player->y = 0;

	player->bounding_box.x = 0;
	player->bounding_box.y = 0;
	player->bounding_box.w = 0;
	player->bounding_box.h = 0;

	player->speed = 0;
	player->angle = 0;
}

void load_player(Player *player)
{
	load_texture(&player->texture, "images/player.png");

	player->max_health = player->health = 56;
	player->selected_skill = 1;

	player->x = 200.0f;
	player->y = 200.0f;

	player->bounding_box.x = 16;
	player->bounding_box.y = 16;
	player->bounding_box.w = 32;
	player->bounding_box.h = 32;

	player->speed = 300.0f;
	player->angle = 0.0f;
}

void draw_player(Player *player)
{
	GLfloat diff_x = mouse_x - player->x - player->texture.image_width / 2 + camera_x;
	GLfloat diff_y = mouse_y - player->y - player->texture.image_height / 2 + camera_y;
	player->angle = 1.57079632679 + atan2(diff_y, diff_x);

	draw_texture(&player->texture, player->x, player->y, NULL, player->angle * 57.2957795);
}

void move_player(Level *level, Player *player)
{
	const uint8_t *key_states = SDL_GetKeyboardState(NULL);

	// TODO: Camera should follow the player.
	if(key_states[SDL_SCANCODE_W] == 1)
	{
		player->y -= player->speed * DELTA_TICKS;

		// NOTE: This is necessary only if the level is not surrounded by walls.
		if(player->y < -player->bounding_box.y)
		{
			player->y = -player->bounding_box.y;
		}

		// FIXME: If the player's bounding box is bigger than a tile, this collision doesn't work.
		if(level->properties[(int)((player->x + player->bounding_box.x) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->properties[(int)((player->x + player->bounding_box.x + player->bounding_box.w) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->y = (int)((player->y + player->bounding_box.y) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->bounding_box.y;
		}
	}
	else if(key_states[SDL_SCANCODE_S] == 1)
	{
		player->y += player->speed * DELTA_TICKS;

		if(player->y >= level->height * BLOCK_SIZE - player->texture.image_height + player->bounding_box.y - 0.25)
		{
			player->y = level->height * BLOCK_SIZE - player->texture.image_height + player->bounding_box.y - 0.25;
		}

		if(level->properties[(int)((player->x + player->bounding_box.x) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y + player->bounding_box.h) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->properties[(int)((player->x + player->bounding_box.x + player->bounding_box.w) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y + player->bounding_box.h) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->y = (int)((player->y + player->bounding_box.y + player->bounding_box.h) / BLOCK_SIZE) * BLOCK_SIZE - player->bounding_box.y - player->bounding_box.h - 0.25;
		}
	}
	if(key_states[SDL_SCANCODE_A] == 1)
	{
		player->x -= player->speed * DELTA_TICKS;

		if(player->x < -player->bounding_box.x)
		{
			player->x = -player->bounding_box.x;
		}

		if(level->properties[(int)((player->x + player->bounding_box.x) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->properties[(int)((player->x + player->bounding_box.x) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y + player->bounding_box.h) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->x = (int)((player->x + player->bounding_box.x) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->bounding_box.x;
		}
	}
	else if(key_states[SDL_SCANCODE_D] == 1)
	{
		player->x += player->speed * DELTA_TICKS;

		if(player->x >= level->width * BLOCK_SIZE - player->texture.image_width + player->bounding_box.x - 0.25)
		{
			player->x = level->width * BLOCK_SIZE - player->texture.image_width + player->bounding_box.x - 0.25;
		}

		if(level->properties[(int)((player->x + player->bounding_box.x + player->bounding_box.w) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->properties[(int)((player->x + player->bounding_box.x + player->bounding_box.w) / BLOCK_SIZE)][(int)((player->y + player->bounding_box.y + player->bounding_box.h) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->x = (int)((player->x + player->bounding_box.x + player->bounding_box.w) / BLOCK_SIZE) * BLOCK_SIZE - player->bounding_box.x - player->bounding_box.w - 0.25;
		}
	}
}
