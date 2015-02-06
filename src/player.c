#include "player.h"

void initPlayer(Player *player)
{
	initTexture(&player->PlayerTexture);
	player->Animations = NULL;
	player->AttackAnimations = NULL;

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

	player->State[RUNNING] = false;
	player->State[STOPPING] = false;
	player->State[JUMPING] = false;
	player->State[ATTACKING] = false;
	player->State[IDLE] = false;

	player->Speed = 0.0f;
	player->CurrentSpeed = 0.0f;

	player->JumpSpeed = 0.0f;
	player->CurrentJumpSpeed = 0.0f;

	player->KeyStates = NULL;
	player->IsFacingLeft = false;

	player->IdleTimer = 0;

	player->InputTextures = NULL;
	player->MaxInputLength = 0;
	player->InputList = NULL;
	player->InputIndex = 0;
	player->InputTicks = 0;
	player->Input = 0;
	player->KeyState = 0;
	player->LastDirection = 0;

	player->InputString[0] = '\0';
	player->InputStringIndex = 0;
	player->InputStringTime = 0;

	player->AttackInput = false;
	player->AttackID = 0;

	// for(int i = 0; i < 4; i++)
	// {
	// 	player->Attacks[0][i] = 0;
	// }

	player->PrevAnimation = 0;
	player->CurrAnimation = 0;
	player->NextAnimation = 0;
}

// TODO: Use a data file instead of playerTexture.
void loadPlayer(Player *player, const char *playerTexture)
{
	loadTexture(&player->PlayerTexture, playerTexture, 0);

	// TODO: Free player->Animations.
	player->Animations = (Animation *)malloc(7 * sizeof(Animation));

	for(int i = 0; i < 7; i++)
	{
		initAnimation(&player->Animations[i]);
	}

	// TODO: Replace integer indices with defines.
	loadAnimation(&player->Animations[0], "images/player/animationIdle.png", 2, 2, 1, 0.45f, true);
	loadAnimation(&player->Animations[1], "images/player/animationStartRunning.png", 6, 3, 2, 0.05f, false);
	loadAnimation(&player->Animations[2], "images/player/animationRunning.png", 12, 4, 3, 0.075f, true);
	loadAnimation(&player->Animations[3], "images/player/animationStopping.png", 5, 3, 2, 0.1f, false);
	loadAnimation(&player->Animations[4], "images/player/animationIdleWait.png", 11, 4, 3, 0.2f, true);
	loadAnimation(&player->Animations[5], "images/player/animationJumping.png", 9, 3, 3, 0.075f, false);
	loadAnimation(&player->Animations[6], "images/player/animationGroundStrike.png", 8, 4, 2, 0.06f, false);

	// TODO: Free player->AttackAnimations.
	player->AttackAnimations = (Animation *)malloc(7 * sizeof(Animation));

	for(int i = 0; i < 1; i++)
	{
		initAnimation(&player->AttackAnimations[i]);
	}

	loadAnimation(&player->AttackAnimations[0], "images/attack/groundStrike.png", 12, 3, 4, 0.1f, false);

	player->MaxHealth = player->Health = 56;
	player->SelectedSkill = 1;

	player->X = 600.0f;
	player->Y = 516.0f;
	player->Scale = 4.0f;

	player->BoundingBox.X = 4.0f;
	player->BoundingBox.Y = 4.0f;
	player->BoundingBox.W = 15.0f;
	player->BoundingBox.H = 22.0f;

	player->State[RUNNING] = false;
	player->State[STOPPING] = false;
	player->State[JUMPING] = false;
	player->State[ATTACKING] = false;

	player->Speed = 400.0f;
	player->CurrentSpeed = 0.0f;

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
	player->State[IDLE] = false;

	player->InputTextures = (Texture *)malloc(9 * sizeof(Texture));

	for(int i = 0; i < 9; i++)
	{
		initTexture(&player->InputTextures[i]);
	}

	loadTexture(&player->InputTextures[0], "images/input/up.png", 0);
	loadTexture(&player->InputTextures[1], "images/input/down.png", 0);
	loadTexture(&player->InputTextures[2], "images/input/left.png", 0);
	loadTexture(&player->InputTextures[3], "images/input/right.png", 0);
	loadTexture(&player->InputTextures[4], "images/input/up_left.png", 0);
	loadTexture(&player->InputTextures[5], "images/input/up_right.png", 0);
	loadTexture(&player->InputTextures[6], "images/input/down_left.png", 0);
	loadTexture(&player->InputTextures[7], "images/input/down_right.png", 0);
	loadTexture(&player->InputTextures[8], "images/input/punch.png", 0);

	player->MaxInputLength = 14;

	player->InputList = (int8_t *)malloc(player->MaxInputLength * sizeof(int8_t));

	for(int i = 0; i < player->MaxInputLength; i++)
	{
		player->InputList[i] = -1;
	}

	player->InputIndex = 0;
	player->InputTicks = SDL_GetTicks();
	player->Input = 0;
	player->KeyState = 0;
	player->LastDirection = 0;

	player->InputString[0] = '\0';
	player->InputStringIndex = 0;
	player->InputStringTime = SDL_GetTicks();

	player->AttackInput = false;
	player->AttackID = -1;

	player->Attacks[0][0] = DOWN;
	player->Attacks[0][1] = DOWN_RIGHT;
	player->Attacks[0][2] = RIGHT;
	player->Attacks[0][3] = PUNCH;

	player->Attacks[1][0] = DOWN;
	player->Attacks[1][1] = DOWN_LEFT;
	player->Attacks[1][2] = LEFT;
	player->Attacks[1][3] = PUNCH;

	player->PrevAnimation = -1;
	player->CurrAnimation = 0;
	player->NextAnimation = -1;
}

void handlePlayerInput(Player *player, SDL_Event *event)
{
	if(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		bool isKeyDown = (event->type == SDL_KEYDOWN);

		switch(event->key.keysym.sym)
		{
			case SDLK_w: case SDLK_UP: case SDLK_SPACE:
				if(event->key.repeat == 0)
				{
					player->KeyStates[2] = isKeyDown;

					if(isKeyDown == true)
					{
						// TODO: Move outside the if conditional using XOR.
						player->KeyState |= UP;
						player->Input |= UP;

						if((player->KeyState & LEFT) != 0)
						{
							player->Input |= LEFT;
						}
						else if((player->KeyState & RIGHT) != 0)
						{
							player->Input |= RIGHT;
						}
					}
					else
					{
						player->KeyState &= ~UP;
						player->Input &= ~UP;

						if((player->KeyState & LEFT) != 0)
						{
							player->Input |= LEFT;
						}
						else if((player->KeyState & RIGHT) != 0)
						{
							player->Input |= RIGHT;
						}

						if((player->KeyState & DOWN) != 0)
						{
							player->Input |= DOWN;
						}
					}
				}
			break;

			case SDLK_s: case SDLK_DOWN:
				if(event->key.repeat == 0)
				{
					if(isKeyDown == true)
					{
						player->KeyState |= DOWN;

						if((player->KeyState & UP) == 0)
						{
							player->Input |= DOWN;

							if((player->KeyState & LEFT) != 0)
							{
								player->Input |= LEFT;
							}
							else if((player->KeyState & RIGHT) != 0)
							{
								player->Input |= RIGHT;
							}
						}
					}
					else
					{
						player->KeyState &= ~DOWN;

						if((player->KeyState & UP) == 0)
						{
							player->Input &= ~DOWN;

							if((player->KeyState & LEFT) != 0)
							{
								player->Input |= LEFT;
							}
							else if((player->KeyState & RIGHT) != 0)
							{
								player->Input |= RIGHT;
							}
						}
					}
				}
			break;

			case SDLK_a: case SDLK_LEFT:
				if(event->key.repeat == 0)
				{
					player->KeyStates[0] = isKeyDown;
					player->IsFacingLeft = true;

					// TODO: Smooth start?
					// TODO: Smooth turning.
					if(player->KeyStates[1] == true)
					{
						player->CurrentSpeed = isKeyDown == true ? -player->Speed : player->Speed;
						player->State[RUNNING] = true;
						player->IsFacingLeft = isKeyDown;
					}
					else
					{
						if(isKeyDown == true)
						{
							player->CurrentSpeed = -player->Speed;
						}

						player->State[RUNNING] = isKeyDown;
						player->State[STOPPING] = !isKeyDown;
					}

					if(isKeyDown == true)
					{
						player->KeyState |= LEFT;
						player->Input |= LEFT;
						player->LastDirection = LEFT;

						if((player->KeyState & UP) != 0)
						{
							player->Input |= UP;
						}
						else if((player->KeyState & DOWN) != 0)
						{
							player->Input |= DOWN;
						}
					}
					else
					{
						player->KeyState &= ~LEFT;
						player->Input &= ~LEFT;

						if((player->KeyState & UP) != 0)
						{
							if((player->KeyState & RIGHT) == 0 || player->LastDirection == LEFT)
							{
								player->Input |= UP;
							}
						}
						else if((player->KeyState & DOWN) != 0)
						{
							if((player->KeyState & RIGHT) == 0 || player->LastDirection == LEFT)
							{
								player->Input |= DOWN;
							}
						}

						if((player->KeyState & RIGHT) != 0)
						{
							if(player->LastDirection == LEFT)
							{
								player->Input |= RIGHT;
							}
						}
					}
				}
			break;

			case SDLK_d: case SDLK_RIGHT:
				if(event->key.repeat == 0)
				{
					player->KeyStates[1] = isKeyDown;
					player->IsFacingLeft = false;

					if(player->KeyStates[0] == true)
					{
						player->CurrentSpeed = isKeyDown ? player->Speed : -player->Speed;
						player->State[RUNNING] = true;
						player->IsFacingLeft = !isKeyDown;
					}
					else
					{
						if(isKeyDown == true)
						{
							player->CurrentSpeed = player->Speed;
						}

						player->State[RUNNING] = isKeyDown;
						player->State[STOPPING] = !isKeyDown;
					}

					if(isKeyDown == true)
					{
						player->KeyState |= RIGHT;
						player->Input |= RIGHT;
						player->LastDirection = RIGHT;

						if((player->KeyState & UP) != 0)
						{
							player->Input |= UP;
						}
						else if((player->KeyState & DOWN) != 0)
						{
							player->Input |= DOWN;
						}
					}
					else
					{
						player->KeyState &= ~RIGHT;
						player->Input &= ~RIGHT;

						if((player->KeyState & UP) != 0)
						{
							if((player->KeyState & LEFT) == 0 || player->LastDirection == RIGHT)
							{
								player->Input |= UP;
							}
						}
						else if((player->KeyState & DOWN) != 0)
						{
							if((player->KeyState & LEFT) == 0 || player->LastDirection == RIGHT)
							{
								player->Input |= DOWN;
							}
						}

						if((player->KeyState & LEFT) != 0)
						{
							if(player->LastDirection == RIGHT)
							{
								player->Input |= LEFT;
							}
						}
					}
				}
			break;

			case SDLK_r:
				if(event->key.repeat == 0 && isKeyDown == true)
				{
					player->Health += 8;
				}

				if(player->Health > player->MaxHealth)
				{
					player->Health = player->MaxHealth;
				}
			break;

			case SDLK_f:
				if(event->key.repeat == 0 && isKeyDown == true)
				{
					player->Health -= 8;
				}

				if(player->Health < 0)
				{
					player->Health = 0;
				}
			break;

			case SDLK_1: case SDLK_2:
			case SDLK_3: case SDLK_4:
				player->SelectedSkill = event->key.keysym.sym - 48;
			break;

			case SDLK_j: case SDLK_k: case SDLK_l: case SDLK_SEMICOLON:
				if(event->key.repeat == 0 && isKeyDown == true)
				{
					// player->State[ATTACKING] = true;
					player->AttackInput = true;
					player->Input |= PUNCH;
				}
			break;

			case SDLK_u: case SDLK_i: case SDLK_o: case SDLK_p:
			break;

			case SDLK_b:
				player->NextAnimation = 2;
			break;

			case SDLK_n:
				player->NextAnimation = 0;
			break;

			case SDLK_m:
				player->NextAnimation = 1;
			break;

			default:
			break;
		}
	}
}

void updatePlayer(Player *player, Level *level)
{
	if(SDL_GetTicks() - player->InputStringTime > 500)
	{
		player->InputString[0] = '\0';
		player->InputStringTime = 0;
		player->InputStringIndex = 0;
	}

	if(SDL_GetTicks() - player->InputTicks >= 16)
	{
		player->InputTicks = SDL_GetTicks();

		// TODO: XOR KeyState with Input.
		if((player->Input & UP) != 0 && (player->Input & LEFT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'Q';
			player->InputList[player->InputIndex] = 4;
		}
		else if((player->Input & UP) != 0 && (player->Input & RIGHT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'E';
			player->InputList[player->InputIndex] = 5;
		}
		else if((player->Input & DOWN) != 0 && (player->Input & LEFT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'Z';
			player->InputList[player->InputIndex] = 6;
		}
		else if((player->Input & DOWN) != 0 && (player->Input & RIGHT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'C';
			player->InputList[player->InputIndex] = 7;
		}
		else if((player->Input & UP) != 0)
		{
			player->InputString[player->InputStringIndex] = 'W';
			player->InputList[player->InputIndex] = 0;
		}
		else if((player->Input & DOWN) != 0)
		{
			player->InputString[player->InputStringIndex] = 'S';
			player->InputList[player->InputIndex] = 1;
		}
		else if((player->Input & LEFT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'A';
			player->InputList[player->InputIndex] = 2;

			// player->CurrAnimation = 1;
			// player->NextAnimation = 2;
		}
		else if((player->Input & RIGHT) != 0)
		{
			player->InputString[player->InputStringIndex] = 'D';
			player->InputList[player->InputIndex] = 3;
		}
		else if((player->Input & PUNCH) != 0)
		{
			player->InputString[player->InputStringIndex] = 'P';
			player->InputList[player->InputIndex] = 8;
		}

		if(player->Input > 0)
		{
			player->InputStringTime = SDL_GetTicks();
			player->InputStringIndex++;
			player->InputString[player->InputStringIndex] = '\0';
			player->InputIndex++;
			player->InputIndex = player->InputIndex % player->MaxInputLength;
			player->Input = 0;
		}

		if(player->AttackInput == true)
		{
			bool match = false;

			int len = strlen(player->InputString);
			int idx = len - 4;

			if(strcmp(&player->InputString[idx], "SCDP") == 0 || strcmp(&player->InputString[idx], "SZAP") == 0)
			{
				match = true;
			}

			if(match == true)
			{
				player->State[ATTACKING] = true;
				// player->AttackID = INDEX;
			}

			player->AttackInput = false;
		}
	}

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

	if(player->State[IDLE] == true && player->CurrentSpeed != 0.0f)
	{
		// stopAnimation(&player->Animations[4]);
		player->IdleTimer = 0;
		player->State[IDLE] = false;
	}

	if(SDL_GetTicks() - player->IdleTimer >= 10000 && player->IdleTimer != 0 && player->State[IDLE] == false)
	{
		player->State[IDLE] = true;
	}

	if(player->IdleTimer == 0 && player->CurrentSpeed == 0.0f && player->State[IDLE] == false)
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

	if(player->State[JUMPING] == true && player->CurrentJumpSpeed < 0.0f)
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

				if(player->State[JUMPING] == true)
				{
					player->State[JUMPING] = false;
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

	if(player->State[STOPPING] == true)
	{
		player->CurrentSpeed *= 1.0f - (9.5f * DELTA_TICKS);

		if(fabs(player->CurrentSpeed) < 25.0f)
		{
			player->CurrentSpeed = 0.0f;
		}
	}

	if(player->KeyStates[2] == true)
	{
		if(player->State[JUMPING] == false && player->CurrentJumpSpeed == 0.0f)
		{
			player->CurrentJumpSpeed = player->JumpSpeed;
			player->State[JUMPING] = true;
			// NOTE: This prevents constant jumping?
			player->KeyStates[2] = false;
		}
	}

	// TODO: Calculate center of the player.
	cameraX = (int)(player->X + player->BoundingBox.X + (player->BoundingBox.W / 2)) - (SCREEN_WIDTH / 2);
	cameraY = (int)(player->Y + player->BoundingBox.Y + (player->BoundingBox.H / 2)) - (SCREEN_HEIGHT / 2);

	// if(player->State[JUMPING] == true && player->CurrAnimation != 5)
	// {
	// 	stopAnimation(&player->Animations[5]);
	// 	player->CurrAnimation = 5;
	// }

	// if(player->State[ATTACKING] == true)
	// {
	// 	startAnimation(&player->AttackAnimations[0], player->IsFacingLeft ? player->X - 200.0f : player->X + 140.0f, player->Y - 20.0f, player->Scale, player->IsFacingLeft);
	// }

	if(player->State[RUNNING] == true)
	{
		// player->CurrAnimation = 1;
		// player->NextAnimation = 2;
	}
	else
	{
		// player->CurrAnimation = 0;
		// player->NextAnimation = 0;
	}

	if(player->Animations[player->CurrAnimation].IsFinished == true || player->Animations[player->CurrAnimation].IsSkippable == true)
	{
		if(player->NextAnimation == -1)
		{
			// stopAnimation(&player->Animations[player->CurrAnimation]);
		}
		else
		{
			stopAnimation(&player->Animations[player->NextAnimation]);
			player->CurrAnimation = player->NextAnimation;
			player->NextAnimation = -1;
		}
	}
}

void drawPlayer(Player *player)
{
	// // TODO: Move to updatePlayer().
	// if(player->State[RUNNING] == true)
	// {
	// 	// FIXME: Jumping animation if the player is jumping.
	// 	stopAnimation(&player->Animations[0]);
	// 	stopAnimation(&player->Animations[4]);

	// 	// FIXME: Change rendering origin for animations.
	// 	if(player->Animations[1].IsFinished == false)
	// 	{
	// 		playAnimation(&player->Animations[1], player->X, player->Y, player->Scale, player->IsFacingLeft);
	// 	}
	// 	else
	// 	{
	// 		playAnimation(&player->Animations[2], player->X, player->Y, player->Scale, player->IsFacingLeft);
	// 	}
	// }
	// else if(player->State[STOPPING] == true)
	// {
	// 	stopAnimation(&player->Animations[0]);
	// 	stopAnimation(&player->Animations[1]);
	// 	stopAnimation(&player->Animations[2]);
	// 	playAnimation(&player->Animations[3], player->X, player->Y, player->Scale, player->IsFacingLeft);

	// 	// NOTE: Animation speed should be synced.
	// 	if(player->CurrentSpeed == 0.0f && player->Animations[3].IsFinished == true)
	// 	{
	// 		stopAnimation(&player->Animations[3]);
	// 		player->State[STOPPING] = false;
	// 	}
	// }
	// else if(player->State[JUMPING] == true)
	// {
	// 	stopAnimation(&player->Animations[0]);
	// 	stopAnimation(&player->Animations[1]);
	// 	stopAnimation(&player->Animations[2]);
	// 	stopAnimation(&player->Animations[3]);
	// 	stopAnimation(&player->Animations[4]);
	// 	playAnimation(&player->Animations[5], player->X, player->Y, player->Scale, player->IsFacingLeft);
	// }
	// else if(player->State[IDLE] == true)
	// {
	// 	playAnimation(&player->Animations[4], player->X, player->Y, player->Scale, player->IsFacingLeft);

	// 	if(player->Animations[4].IsFinished == true)
	// 	{
	// 		stopAnimation(&player->Animations[4]);
	// 		player->IdleTimer = 0;
	// 		player->State[IDLE] = false;
	// 	}
	// }
	// else if(player->State[ATTACKING] == true)
	// {
	// 	stopAnimation(&player->Animations[0]);
	// 	stopAnimation(&player->Animations[2]);
	// 	playAnimation(&player->Animations[6], player->X, player->Y + 12.0f, player->Scale, player->IsFacingLeft);
	// 	startAnimation(&player->AttackAnimations[0], player->IsFacingLeft ? player->X - 200.0f : player->X + 140.0f, player->Y - 20.0f, player->Scale, player->IsFacingLeft);

	// 	if(player->Animations[6].IsFinished == true)
	// 	{
	// 		stopAnimation(&player->Animations[6]);
	// 		player->State[ATTACKING] = false;
	// 		player->AttackID = -1;
	// 	}
	// }
	// else
	// {
	// 	stopAnimation(&player->Animations[1]);
	// 	stopAnimation(&player->Animations[2]);
	// 	stopAnimation(&player->Animations[5]);
	// 	playAnimation(&player->Animations[0], player->X, player->Y, player->Scale, player->IsFacingLeft);
	// }

	if(player->AttackAnimations[0].Started == true)
	{
		playStaticAnimation(&player->AttackAnimations[0]);

		if(player->AttackAnimations[0].IsFinished == true)
		{
			stopAnimation(&player->AttackAnimations[0]);
		}
	}

	playAnimation(&player->Animations[player->CurrAnimation], player->X, player->Y, player->Scale, player->IsFacingLeft);

	if(DEBUG == true)
	{
		drawEmptyRectangle(player->X, player->Y, player->PlayerTexture.Width * player->Scale, player->PlayerTexture.Height * player->Scale, 1.0f, 0xFFFFFF);
		drawRectangle(player->X + (player->BoundingBox.X * player->Scale), player->Y + (player->BoundingBox.Y * player->Scale), player->BoundingBox.W * player->Scale, player->BoundingBox.H * player->Scale, 0xFFFFFF, 1.0f);
	}
}

void drawPlayerInput(Player *player)
{
	// TODO: Draw in reverse order.
	uint8_t i = player->InputIndex;
	uint8_t position = 0;

	do
	{
		if(player->InputList[i] != -1)
		{
			drawTexture(&player->InputTextures[player->InputList[i]], 10.0f, 60.0f + (position++ * 40.0f), NULL, 0.0f, 2.0f, false);
		}

		i = (i + 1) % player->MaxInputLength;
	} while(i != player->InputIndex);
}
