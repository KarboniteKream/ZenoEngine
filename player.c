#include "player.h"

void initPlayer(Player *player)
{
	initTexture(&player->PlayerTexture);
	player->Animations = NULL;
	player->Attacks = NULL;

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

	player->IsAttacking = false;
}

// TODO: Use a data file instead of playerTexture.
void loadPlayer(Player *player, const char *playerTexture)
{
	loadTexture(&player->PlayerTexture, playerTexture);

	// TODO: Free player->Animations.
	player->Animations = (Animation *)malloc(7 * sizeof(Animation));

	for(int i = 0; i < 7; i++)
	{
		initAnimation(&player->Animations[i]);
	}

	loadAnimation(&player->Animations[0], "images/animationIdle.png", 2, 2, 1, 0.45f);
	loadAnimation(&player->Animations[1], "images/animationStartRunning.png", 6, 3, 2, 0.05f);
	loadAnimation(&player->Animations[2], "images/animationRunning.png", 12, 4, 3, 0.075f);
	loadAnimation(&player->Animations[3], "images/animationStopping.png", 5, 3, 2, 0.1f);
	loadAnimation(&player->Animations[4], "images/animationIdleWait.png", 11, 4, 3, 0.2f);
	loadAnimation(&player->Animations[5], "images/animationJumping.png", 9, 3, 3, 0.075f);
	loadAnimation(&player->Animations[6], "images/groundStrikePlayerAnimation.png", 8, 4, 2, 0.06f);

	// TODO: Free player->Attacks.
	player->Attacks = (Animation *)malloc(7 * sizeof(Animation));

	for(int i = 0; i < 1; i++)
	{
		initAnimation(&player->Attacks[i]);
	}

	loadAnimation(&player->Attacks[0], "images/groundStrikeAnimation.png", 12, 3, 4, 0.1f);

	player->MaxHealth = player->Health = 56;
	player->SelectedSkill = 1;

	player->X = 600.0f;
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

	player->IsAttacking = false;
}

void handlePlayerEvent(Player *player, SDL_Event *event)
{
	if(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
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
						player->CurrentSpeed = keyState == true ? -player->Speed : player->Speed;
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

			case SDLK_w: case SDLK_SPACE:
				if(event->key.repeat == 0)
				{
					player->KeyStates[2] = keyState;
				}
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

			case SDLK_q:
				player->IsAttacking = true;
			break;

			default:
			break;
		}
	}
}

void updatePlayer(Player *player, Level *level)
{
	player->X += player->CurrentSpeed * DELTA_TICKS;

	// NOTE: This is necessary only if the level is not surrounded by walls.
	if(player->X < -(player->BoundingBox.X * player->Scale))
	{
		player->X = -(player->BoundingBox.X * player->Scale);
	}
	else if(player->X > level->Width * BLOCK_SIZE - player->PlayerTexture.Width * player->Scale - 0.25f)
	{
		player->X = level->Width * BLOCK_SIZE - player->PlayerTexture.Width * player->Scale - 0.25f;
	}

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

	//NOTE: Collision only needs four points (vertices/edges). There is no need to calculate eight of them.
	if(player->IsFacingLeft == true)
	{
		int w = (player->X + (player->BoundingBox.X * player->Scale)) / BLOCK_SIZE;
		int h1 = (player->Y + (player->BoundingBox.Y * player->Scale)) / BLOCK_SIZE;
		int h2 = (player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE;

		for(int i = h1; i <= h2; i++)
		{
			if(level->Data[w][i][PROPERTIES] == COLLIDABLE)
			{
				player->X = (w + 1) * BLOCK_SIZE - (player->BoundingBox.X * player->Scale);
				break;
			}
			else if(level->Data[w][i][PROPERTIES] == SLOPE_LEFT)
			{
				// NOTE: Should I check only h2?
				GLfloat w1 = (player->X + (player->BoundingBox.X * player->Scale));
				GLfloat w2 = w1 - (int)(w1 / BLOCK_SIZE) * BLOCK_SIZE;
				player->Y = (i - 2) * BLOCK_SIZE + w2;
			}
		}
	}
	else
	{
		int w = (player->X + ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale)) / BLOCK_SIZE;
		int h1 = (player->Y + (player->BoundingBox.Y * player->Scale)) / BLOCK_SIZE;
		int h2 = (player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE;

		for(int i = h1; i <= h2; i++)
		{
			if(level->Data[w][i][PROPERTIES] == COLLIDABLE)
			{
				player->X = w * BLOCK_SIZE - ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale) - 0.25;
				break;
			}
			else if(level->Data[w][i][PROPERTIES] == SLOPE_RIGHT)
			{
				// NOTE: Should I check only h2?
				GLfloat w1 = (player->X + ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale));
				// NOTE: Check only the left side of the block.
				GLfloat w2 = -(((int)(w1 / BLOCK_SIZE) + 1) * BLOCK_SIZE - w1);
				player->Y = (i - 2) * BLOCK_SIZE + w2;
			}
		}
	}

	player->Y += player->CurrentJumpSpeed * DELTA_TICKS;

	if(player->Y < -player->BoundingBox.Y)
	{
		player->Y = -player->BoundingBox.Y;
	}
	else if(player->Y > level->Height * BLOCK_SIZE - player->PlayerTexture.Height * player->Scale - player->BoundingBox.Y * player->Scale - 0.25f)
	{
		player->Y = level->Height * BLOCK_SIZE - player->PlayerTexture.Height * player->Scale - player->BoundingBox.Y * player->Scale - 0.25f;
		player->CurrentJumpSpeed = 0.0f;
	}

	if(player->IsJumping == true && player->CurrentJumpSpeed < 0.0f)
	{
		player->CurrentJumpSpeed += 1600.0f * DELTA_TICKS;
	}
	else
	{
		// TODO: Rename to 'falling'.
		// TODO: Falling animation.
		player->CurrentJumpSpeed += 2500.0f * DELTA_TICKS;
	}

	if(player->CurrentJumpSpeed >= 0.0f)
	{
		int h = (player->Y + ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale)) / BLOCK_SIZE;
		int w1 = (player->X + (player->BoundingBox.X * player->Scale)) / BLOCK_SIZE;
		int w2 = (player->X + ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale)) / BLOCK_SIZE;

		for(int i = w1; i <= w2; i++)
		{
			if(level->Data[i][h][PROPERTIES] == COLLIDABLE)
			{
				player->Y = h * BLOCK_SIZE - ((player->BoundingBox.Y + player->BoundingBox.H) * player->Scale) - 0.25;

				if(player->IsJumping == true)
				{
					player->IsJumping = false;
				}

				player->CurrentJumpSpeed = 0.0f;

				break;
			}
			else if(level->Data[i][h][PROPERTIES] == SLOPE_LEFT)
			{
				player->CurrentJumpSpeed = 0.0f;
			}
		}
	}
	else
	{
		int h = (player->Y + (player->BoundingBox.Y * player->Scale)) / BLOCK_SIZE;
		int w1 = (player->X + (player->BoundingBox.X * player->Scale)) / BLOCK_SIZE;
		int w2 = (player->X + ((player->BoundingBox.X + player->BoundingBox.W) * player->Scale)) / BLOCK_SIZE;

		for(int i = w1; i <= w2; i++)
		{
			if(level->Data[i][h][PROPERTIES] == COLLIDABLE)
			{
				player->Y = (h + 1) * BLOCK_SIZE - (player->BoundingBox.Y * player->Scale);
				// TODO: Stop jumping animation.
				player->CurrentJumpSpeed = 0.0f;

				break;
			}
		}
	}

	if(player->IsStopping == true)
	{
		player->CurrentSpeed *= 1.0f - (9.5f * DELTA_TICKS);

		if(fabs(player->CurrentSpeed) < 25.0f)
		{
			player->CurrentSpeed = 0.0f;
		}
	}

	if(player->KeyStates[2] == true)
	{
		if(player->IsJumping == false && player->CurrentJumpSpeed == 0.0f)
		{
			player->CurrentJumpSpeed = player->JumpSpeed;
			player->IsJumping = true;
			player->KeyStates[2] = false;
		}
	}

	// TODO: Calculate center of the player.
	cameraX = (int)(player->X + player->BoundingBox.X + (player->BoundingBox.W / 2)) - (SCREEN_WIDTH / 2);
	cameraY = (int)(player->Y + player->BoundingBox.Y + (player->BoundingBox.H / 2)) - (SCREEN_HEIGHT / 2);
}

void drawPlayer(Player *player)
{
	// NOTE: Should this be moved to update?
	if(player->IsMoving == true)
	{
		// FIXME: Jumping animation if the player is jumping.
		stopAnimation(&player->Animations[0]);
		stopAnimation(&player->Animations[4]);

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
		playAnimation(&player->Animations[3], player->X, player->Y, player->Scale, player->IsFacingLeft);

		// NOTE: Animation speed should be synced.
		if(player->CurrentSpeed == 0.0f && player->Animations[3].IsFinished == true)
		{
			stopAnimation(&player->Animations[3]);
			player->IsStopping = false;
		}
	}
	else if(player->IsJumping == true)
	{
		stopAnimation(&player->Animations[0]);
		stopAnimation(&player->Animations[1]);
		stopAnimation(&player->Animations[2]);
		stopAnimation(&player->Animations[3]);
		stopAnimation(&player->Animations[4]);
		playAnimation(&player->Animations[5], player->X, player->Y, player->Scale, player->IsFacingLeft);
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
	else if(player->IsAttacking == true)
	{
		stopAnimation(&player->Animations[0]);
		stopAnimation(&player->Animations[2]);
		playAnimation(&player->Animations[6], player->X, player->Y + 12.0f, player->Scale, player->IsFacingLeft);
		startAnimation(&player->Attacks[0], player->IsFacingLeft ? player->X - 200.0f : player->X + 140.0f, player->Y - 20.0f, player->Scale, player->IsFacingLeft);

		if(player->Animations[6].IsFinished == true)
		{
			stopAnimation(&player->Animations[6]);
			player->IsAttacking = false;
		}
	}
	else
	{
		stopAnimation(&player->Animations[1]);
		stopAnimation(&player->Animations[2]);
		stopAnimation(&player->Animations[5]);
		playAnimation(&player->Animations[0], player->X, player->Y, player->Scale, player->IsFacingLeft);
	}

	if(player->Attacks[0].Started == true)
	{
		playStaticAnimation(&player->Attacks[0]);

		if(player->Attacks[0].IsFinished == true)
		{
			stopAnimation(&player->Attacks[0]);
		}
	}

	if(DEBUG == true)
	{
		drawEmptyRectangle(player->X, player->Y, player->PlayerTexture.Width * player->Scale, player->PlayerTexture.Height * player->Scale, 1.0f, 1.0f, 1.0f, 1.0f);
		drawRectangle(player->X + (player->BoundingBox.X * player->Scale), player->Y + (player->BoundingBox.Y * player->Scale), player->BoundingBox.W * player->Scale, player->BoundingBox.H * player->Scale, 1.0f, 1.0f, 1.0f, 1.0f);
	}
}
