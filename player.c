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

	player->IsMoving = false;
	player->IsStopping = false;
	player->Speed = 0.0f;
	player->CurrentSpeed = 0.0f;

	player->IsJumping = false;
	player->JumpSpeed = 0.0f;
	player->CurrentJumpSpeed = 0.0f;

	player->KeyStates = NULL;
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

	player->IsMoving = false;
	player->IsStopping = false;
	player->Speed = 400.0f;
	player->CurrentSpeed = 0.0f;

	player->IsJumping = false;
	player->JumpSpeed = -600.0f;
	player->CurrentJumpSpeed = 0.0f;

	player->KeyStates = (bool *)malloc(4 * sizeof(bool));
	for(int i = 0; i < 4; i++)
	{
		player->KeyStates[i] = false;
	}
}

void drawPlayer(Player *player)
{
	// GLfloat diffX = mouseX - player->X - player->PlayerTexture.Width / 2 + cameraX;
	// GLfloat diffY = mouseY - player->Y - player->PlayerTexture.Height / 2 + cameraY;
	// player->Angle = 1.57079632679 + atan2(diffY, diffX);
	// player->Angle * 57.2957795

	if(player->IsMoving == true)
	{
		// TODO: Stop idle animation.
		drawTexture(&player->PlayerTexture, player->X, player->Y, NULL, 0.0f, player->Scale);
	}
	else
	{
		playAnimation(&player->Animations[0], player->X, player->Y, player->Scale);
	}
}

void handlePlayerEvent(Player *player, SDL_Event *event)
{
	bool keyState = (event->type == SDL_KEYDOWN) ? true : false;

	switch(event->key.keysym.sym)
	{
		case SDLK_a:
			if(event->key.repeat == 0)
			{
				player->KeyStates[0] = keyState;
				player->IsMoving = keyState;

				// TODO: Smooth start?
				// TODO: Smooth turning.
				if(player->KeyStates[1] == true)
				{
					player->CurrentSpeed = keyState ? -player->Speed : player->Speed;
				}
				else
				{
					if(keyState == true)
					{
						player->CurrentSpeed = -player->Speed;
					}

					player->IsStopping = !keyState;
				}
			}
		break;

		case SDLK_d:
			if(event->key.repeat == 0)
			{
				player->KeyStates[1] = keyState;
				player->IsMoving = keyState;

				if(player->KeyStates[0] == true)
				{
					player->CurrentSpeed = keyState ? player->Speed : -player->Speed;
				}
				else
				{
					if(keyState == true)
					{
						player->CurrentSpeed = player->Speed;
					}

					player->IsStopping = !keyState;
				}
			}
		break;

		case SDLK_w:
			player->KeyStates[2] = keyState;
		break;

		case SDLK_j:
			player->Health -= 4;
			if(player->Health < 0)
			{
				player->Health = 0;
			}
		break;

		case SDLK_k:
			player->Health += 4;
			if(player->Health > player->MaxHealth)
			{
				player->Health = player->MaxHealth;
			}
		break;

		case SDLK_1: case SDLK_2:
		case SDLK_3: case SDLK_4:
			player->SelectedSkill = event->key.keysym.sym - 48;
		break;

		default:
		break;
	}
}

void updatePlayer(Player *player, Level *level)
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

	// TODO: Camera should follow the player.

	if(player->IsStopping == true)
	{
		player->CurrentSpeed *= 1.0f - (9.7f * DELTA_TICKS);

		if(fabs(player->CurrentSpeed) < 25.0f)
		{
			player->CurrentSpeed = 0.0f;
		}
	}

	player->X += player->CurrentSpeed * DELTA_TICKS;

	if(player->KeyStates[2] == true)
	{
		// FIXME: One button press should trigger only one jump.
		if(player->IsJumping == false)
		{
			player->JumpSpeed = -600.0f;
			player->IsJumping = true;
		}
	}

	// NOTE: This is necessary only if the level is not surrounded by walls.
	if(player->X < -player->BoundingBox.X)
	{
		player->X = -player->BoundingBox.X;
	}

	/*if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
	{
		player->X = (int)((player->X + player->BoundingBox.X) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.X;
	}*/

	if(player->X >= level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25)
	{
		player->X = level->Width * BLOCK_SIZE - player->PlayerTexture.Width + player->BoundingBox.X - 0.25;
	}

	/*if(level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
		level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
	{
		player->X = (int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE) * BLOCK_SIZE - player->BoundingBox.X - player->BoundingBox.W - 0.25;
	}*/

	if(player->Y < -player->BoundingBox.Y)
	{
		player->Y = -player->BoundingBox.Y;
	}

	/*if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
		level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE)][COLLIDABLE] == 1)
	{
		player->Y = (int)((player->Y + player->BoundingBox.Y) / BLOCK_SIZE + 1) * BLOCK_SIZE - player->BoundingBox.Y;
	}*/

	if(player->Y >= level->Height * BLOCK_SIZE - player->PlayerTexture.Height + player->BoundingBox.Y - 0.25)
	{
		player->Y = level->Height * BLOCK_SIZE - player->PlayerTexture.Height + player->BoundingBox.Y - 0.25;
	}

	/*if(level->Properties[(int)((player->X + player->BoundingBox.X) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
			level->Properties[(int)((player->X + player->BoundingBox.X + player->BoundingBox.W) / BLOCK_SIZE)][(int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE)][COLLIDABLE] == 1)
	{
		player->Y = (int)((player->Y + player->BoundingBox.Y + player->BoundingBox.H) / BLOCK_SIZE) * BLOCK_SIZE - player->BoundingBox.Y - player->BoundingBox.H - 0.25;
	}*/
}
