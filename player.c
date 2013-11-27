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
	player->IsFacingLeft = false;

	player->IdleTimer = 0;
	player->IsIdle = false;
}

// TODO: Use a data file instead of playerTexture.
void loadPlayer(Player *player, const char *playerTexture)
{
	loadTexture(&player->PlayerTexture, playerTexture);

	// TODO: Free player->Animations.
	player->Animations = (Animation *)malloc(5 * sizeof(Animation));

	for(int i = 0; i < 5; i++)
	{
		initAnimation(&player->Animations[i]);
	}

	loadAnimation(&player->Animations[0], "images/animationIdle.png", 2, 2, 1, 0.45f);
	loadAnimation(&player->Animations[1], "images/animationStartRunning.png", 6, 3, 2, 0.05f);
	loadAnimation(&player->Animations[2], "images/animationRunning.png", 12, 4, 3, 0.075f);
	loadAnimation(&player->Animations[3], "images/animationStopping.png", 5, 3, 2, 0.1f);
	loadAnimation(&player->Animations[4], "images/animationIdleWait.png", 11, 4, 3, 0.2f);

	player->MaxHealth = player->Health = 56;
	player->SelectedSkill = 1;

	player->X = 400.0f;
	player->Y = 516.0f;
	player->Scale = 4.0f;

	player->BoundingBox.X = 4.0f;
	player->BoundingBox.Y = 4.0f;
	player->BoundingBox.W = 15.0f;
	player->BoundingBox.H = 22.0f;

	player->IsMoving = false;
	player->IsStopping = false;
	player->Speed = 400.0f;
	player->CurrentSpeed = 0.0f;

	player->IsJumping = false;
	player->JumpSpeed = -600.0f;
	player->CurrentJumpSpeed = 0.0f;

	// TODO: Free player->KeyStates.
	player->KeyStates = (bool *)malloc(4 * sizeof(bool));
	for(int i = 0; i < 4; i++)
	{
		player->KeyStates[i] = false;
	}

	player->IsFacingLeft = false;

	player->IdleTimer = 0;
	player->IsIdle = false;
}

void drawPlayer(Player *player)
{
	if(player->IsMoving == true)
	{
		stopAnimation(&player->Animations[0]);

		// FIXME: Change rendering origin for animations.
		if(player->Animations[1].IsFinished == false)
		{
			playAnimation(&player->Animations[1], player->X, player->Y, player->Scale, player->IsFacingLeft);
		}
		else
		{
			playAnimation(&player->Animations[2], player->X, player->Y, player->Scale, player->IsFacingLeft);
		}
	}
	else if(player->IsStopping == true)
	{
		stopAnimation(&player->Animations[0]);
		stopAnimation(&player->Animations[1]);
		stopAnimation(&player->Animations[2]);
		// FIXME: Remember last direction, reset animations on change.
		playAnimation(&player->Animations[3], player->X, player->Y, player->Scale, player->IsFacingLeft);

		// NOTE: Animation speed should be synced.
		if(player->CurrentSpeed == 0.0f && player->Animations[3].IsFinished == true)
		{
			stopAnimation(&player->Animations[3]);
			player->IsStopping = false;
		}
	}
	else if(player->IsIdle == true)
	{
		playAnimation(&player->Animations[4], player->X, player->Y, player->Scale, player->IsFacingLeft);

		if(player->Animations[4].IsFinished == true)
		{
			stopAnimation(&player->Animations[4]);
			player->IdleTimer = 0;
			player->IsIdle = false;
		}
	}
	else
	{
		stopAnimation(&player->Animations[1]);
		stopAnimation(&player->Animations[2]);
		playAnimation(&player->Animations[0], player->X, player->Y, player->Scale, player->IsFacingLeft);
	}

	// NOTE: Bounding box debugging.
	//drawEmptyRectangle(player->X, player->Y, player->PlayerTexture.Width * player->Scale, player->PlayerTexture.Height * player->Scale, 1.0f, 1.0f, 1.0f, 1.0f);
	//drawRectangle(player->X + (player->BoundingBox.X * player->Scale), player->Y + (player->BoundingBox.Y * player->Scale), player->BoundingBox.W * player->Scale, player->BoundingBox.H * player->Scale, 1.0f, 1.0f, 1.0f, 1.0f);
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
				player->IsFacingLeft = true;

				// TODO: Smooth start?
				// TODO: Smooth turning.
				if(player->KeyStates[1] == true)
				{
					player->CurrentSpeed = keyState ? -player->Speed : player->Speed;
					player->IsMoving = true;
					player->IsFacingLeft = keyState;
				}
				else
				{
					if(keyState == true)
					{
						player->CurrentSpeed = -player->Speed;
					}

					player->IsStopping = !keyState;
					player->IsMoving = keyState;
				}
			}
		break;

		case SDLK_d:
			if(event->key.repeat == 0)
			{
				player->KeyStates[1] = keyState;
				player->IsFacingLeft = false;

				if(player->KeyStates[0] == true)
				{
					player->CurrentSpeed = keyState ? player->Speed : -player->Speed;
					player->IsMoving = true;
					player->IsFacingLeft = !keyState;
				}
				else
				{
					if(keyState == true)
					{
						player->CurrentSpeed = player->Speed;
					}

					player->IsMoving = keyState;
					player->IsStopping = !keyState;
				}
			}
		break;

		case SDLK_w: case  SDLK_SPACE:
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
	if(player->IsIdle == true && player->CurrentSpeed != 0.0f)
	{
		stopAnimation(&player->Animations[4]);
		player->IdleTimer = 0;
		player->IsIdle = false;
	}

	if(SDL_GetTicks() - player->IdleTimer >= 10000 && player->IdleTimer != 0 && player->IsIdle == false)
	{
		player->IsIdle = true;
	}

	if(player->IdleTimer == 0 && player->CurrentSpeed == 0.0f && player->IsIdle == false)
	{
		player->IdleTimer = SDL_GetTicks();
	}

	player->Y += player->CurrentJumpSpeed * DELTA_TICKS;

	if(player->IsJumping == true && player->CurrentJumpSpeed < 0.0f)
	{
		player->CurrentJumpSpeed += 1600.0f * DELTA_TICKS;
	}
	else
	{
		// TODO: Rename to 'falling'.
		player->CurrentJumpSpeed += 2000.0f * DELTA_TICKS;
	}

	if(level->Properties[(int)((player->X + (player->BoundingBox.X * player->Scale)) / BLOCK_SIZE)][(int)((player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE)][COLLIDABLE] == 1 ||
		level->Properties[(int)((player->X + ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale)) / BLOCK_SIZE)][(int)((player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE)][COLLIDABLE] == 1)
	{
		player->Y = (int)((player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE) * BLOCK_SIZE - ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale);

		if(player->IsJumping == true)
		{
			player->IsJumping = false;
		}

		player->CurrentJumpSpeed = 0.0f;
	}

	// TODO: Camera should follow the player.

	if(player->IsStopping == true)
	{
		player->CurrentSpeed *= 1.0f - (9.5f * DELTA_TICKS);

		if(fabs(player->CurrentSpeed) < 25.0f)
		{
			player->CurrentSpeed = 0.0f;
		}
	}

	player->X += player->CurrentSpeed * DELTA_TICKS;

	if(player->KeyStates[2] == true)
	{
		// FIXME: One button press should trigger only one jump.
		if(player->IsJumping == false && player->CurrentJumpSpeed == 0.0f)
		{
			player->CurrentJumpSpeed = player->JumpSpeed;
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

	if(player->Y >= level->Height * BLOCK_SIZE - player->PlayerTexture.Height * player->Scale + (player->BoundingBox.Y * player->Scale) - 0.25)
	{
		player->Y = level->Height * BLOCK_SIZE - player->PlayerTexture.Height * player->Scale + (player->BoundingBox.Y * player->Scale) - 0.25;
	}
}
