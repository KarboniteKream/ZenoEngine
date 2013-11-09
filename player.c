#include "player.h"

void initPlayer(Player *player)
{
	initTexture(&player->PlayerTexture);
	player->Animations = NULL;

	player->MaxHealth = 0;
	player->Health = 0;
	player->SelectedSkill = 0;

	player->X = 0.0f;
	player->Y = 0.0f;
	player->Scale = 0.0f;

	player->BoundingBox.X = 0.0f;
	player->BoundingBox.Y = 0.0f;
	player->BoundingBox.W = 0.0f;
	player->BoundingBox.H = 0.0f;

	player->Speed = 0.0f;
	player->JumpSpeed = 0.0f;

	player->IsJumping = false;
	player->IsMoving = false;
}

// TODO: Use a data file instead of an image.
void loadPlayer(Player *player, const char *dataFile)
{
	loadTexture(&player->PlayerTexture, dataFile);
	player->Animations = (Animation *)malloc(1 * sizeof(Animation));

	for(int i = 0; i < 1; i++)
	{
		initAnimation(&player->Animations[i]);
		loadAnimation(&player->Animations[i], "images/animationIdle.png", 0.55f);
	}

	player->MaxHealth = player->Health = 56;
	player->SelectedSkill = 1;

	player->X = 600.0f;
	player->Y = 560.0f;
	player->Scale = 4.0f;

	player->BoundingBox.X = 16;
	player->BoundingBox.Y = 16;
	player->BoundingBox.W = 32;
	player->BoundingBox.H = 32;

	player->Speed = 400.0f;
	player->JumpSpeed = 0.0f;

	player->IsJumping = false;
	player->IsMoving = false;
}

void drawPlayer(Player *player)
{
	// GLfloat diffX = mouseX - player->X - player->PlayerTexture.Width / 2 + cameraX;
	// GLfloat diffY = mouseY - player->Y - player->PlayerTexture.Height / 2 + cameraY;
	// player->Angle = 1.57079632679 + atan2(diffY, diffX);

	if(player->IsMoving == true)
	{
		drawTexture(&player->PlayerTexture, player->X, player->Y, NULL, /* player->Angle * 57.2957795 */ 0.0f, player->Scale);
	}
	else
	{
		playAnimation(&player->Animations[0], player->X, player->Y, player->Scale);
	}
}

void movePlayer(Level *level, Player *player)
{
	if(player->IsJumping == true)
	{
		player->Y += player->JumpSpeed * DELTA_TICKS;
		player->JumpSpeed += 1600.0 * DELTA_TICKS;

		if(player->Y > SCREEN_HEIGHT - 160.0f)
		{
			player->IsJumping = false;
			player->Y = SCREEN_HEIGHT - 160.0f;
		}
	}

	// TODO: Store player controls in an array and access them using enumerators.
	const uint8_t *keyStates = SDL_GetKeyboardState(NULL);

	// TODO: Camera should follow the player.
	if(keyStates[SDL_SCANCODE_W] == 1)
	{
		// FIXME: One button press should trigger only one jump.
		if(player->IsJumping == false)
		{
			player->JumpSpeed = -600.0f;
			player->IsJumping = true;
		}

		//player->Y -= player->Speed * DELTA_TICKS;

		// NOTE: This is necessary only if the level is not surrounded by walls.
		if(player->Y < -player->BoundingBox.Y)
		{
			player->Y = -player->BoundingBox.Y;
		}

		/*if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->Y = (int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.Y;
		}*/
	}
	/*else if(keyStates[SDL_SCANCODE_S] == 1)
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
	}*/
	if(keyStates[SDL_SCANCODE_A] == 1)
	{
		player->IsMoving = true;
		player->X -= player->Speed * DELTA_TICKS;

		if(player->X < -player->BoundingBox.X)
		{
			player->X = -player->BoundingBox.X;
		}

		/*if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->X = (int)((player->X + player->BoundingBox.X) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.X;
		}*/
	}
	else if(keyStates[SDL_SCANCODE_D] == 1)
	{
		player->IsMoving = true;
		player->X += player->Speed * DELTA_TICKS;

		if(player->X >= level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25)
		{
			player->X = level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25;
		}

		/*if(level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
		{
			player->X = (int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE) * BLOCK_SIZE - player->BoundingBox.X - player->BoundingBox.W - 0.25;
		}*/
	}
}
