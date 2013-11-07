#include "player.h"

void initPlayer(Player *player)
{
	initTexture(&player->PlayerTexture);

	player->MaxHealth = 0;
	player->Health = 0;
	player->SelectedSkill = 0;

	player->X = 0;
	player->Y = 0;

	player->BoundingBox.X = 0;
	player->BoundingBox.Y = 0;
	player->BoundingBox.W = 0;
	player->BoundingBox.H = 0;

	player->Speed = 0;
	player->Angle = 0;
}

void loadPlayer(Player *player)
{
	loadTexture(&player->PlayerTexture, "images/player.png");

	player->MaxHealth = player->Health = 56;
	player->SelectedSkill = 1;

	player->X = 200.0f;
	player->Y = 200.0f;

	player->BoundingBox.X = 16;
	player->BoundingBox.Y = 16;
	player->BoundingBox.W = 32;
	player->BoundingBox.H = 32;

	player->Speed = 300.0f;
	player->Angle = 0.0f;
}

void drawPlayer(Player *player)
{
	GLfloat diffX = mouseX - player->X - player->PlayerTexture.Width / 2 + cameraX;
	GLfloat diffY = mouseY - player->Y - player->PlayerTexture.Height / 2 + cameraY;
	player->Angle = 1.57079632679 + atan2(diffY, diffX);

	drawTexture(&player->PlayerTexture, player->X, player->Y, NULL, player->Angle * 57.2957795, 1.0f);
}

void movePlayer(Level *level, Player *player)
{
	const uint8_t *keyStates = SDL_GetKeyboardState(NULL);

	// TODO: Camera should follow the player.
	if(keyStates[SDL_SCANCODE_W] == 1)
	{
		player->Y -= player->Speed * DELTA_TICKS;

		// NOTE: This is necessary only if the level is not surrounded by walls.
		if(player->Y < -player->BoundingBox.Y)
		{
			player->Y = -player->BoundingBox.Y;
		}

		// FIXME: If the player's bounding box is bigger than a tile, this collision doesn't work.
		if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->Y = (int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.Y;
		}
	}
	else if(keyStates[SDL_SCANCODE_S] == 1)
	{
		player->Y += player->Speed * DELTA_TICKS;

		if(player->Y >= level->Height * BLOCK_SIZE - player->PlayerTexture.Height + player->BoundingBox.Y - 0.25)
		{
			player->Y = level->Height * BLOCK_SIZE - player->PlayerTexture.Height + player->BoundingBox.Y - 0.25;
		}

		if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->Y = (int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE) * BLOCK_SIZE - player->BoundingBox.Y - player->BoundingBox.H - 0.25;
		}
	}
	if(keyStates[SDL_SCANCODE_A] == 1)
	{
		player->X -= player->Speed * DELTA_TICKS;

		if(player->X < -player->BoundingBox.X)
		{
			player->X = -player->BoundingBox.X;
		}

		if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->X = (int)((player->X + player->BoundingBox.X) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.X;
		}
	}
	else if(keyStates[SDL_SCANCODE_D] == 1)
	{
		player->X += player->Speed * DELTA_TICKS;

		if(player->X >= level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25)
		{
			player->X = level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25;
		}

		if(level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->X = (int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE) * BLOCK_SIZE - player->BoundingBox.X - player->BoundingBox.W - 0.25;
		}
	}
}
