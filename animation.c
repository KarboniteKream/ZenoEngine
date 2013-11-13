#include "animation.h"

void initAnimation(Animation *animation)
{
	initTexture(&animation->AnimationTexture);

	animation->TotalFrames = 0;
	animation->FramesPerRow = 0;
	animation->FramesPerColumn = 0;

	animation->CurrentFrame = 0;
	animation->AnimationSpeed = 0.0f;
	animation->FrameStartTime = 0;
	animation->Started = false;

	animation->Frame.X = 0.0f;
	animation->Frame.Y = 0.0f;
	animation->Frame.W = 0.0f;
	animation->Frame.H = 0.0f;
}

void loadAnimation(Animation *animation, const char *animationTexture, float animationSpeed)
{
	loadTexture(&animation->AnimationTexture, animationTexture);

	animation->TotalFrames = 2;
	animation->FramesPerRow = 2;
	animation->FramesPerColumn = 1;

	animation->CurrentFrame = 0;
	animation->AnimationSpeed = 1.0f - animationSpeed;
	animation->FrameStartTime = 0;
	animation->Started = false;

	animation->Frame.X = 0.0f;
	animation->Frame.Y = 0.0f;
	animation->Frame.W = 19.0f;
	animation->Frame.H = 23.0f;
}

void playAnimation(Animation *animation, GLfloat x, GLfloat y, GLfloat scale)
{
	if(animation->Started == false)
	{
		animation->FrameStartTime = SDL_GetTicks();
		animation->Started = true;
	}

	// TODO: Store in struct instead of calculating every frame.
	//animation->Frame.X = (animation->AnimationTexture.Width / animation->FramesPerRow) * (animation->CurrentFrame % animation->FramesPerRow);
	//animation->Frame.Y = (animation->AnimationTexture.Height / animation->FramesPerColumn) * (int)(animation->CurrentFrame / animation->FramesPerColumn);

	switch(animation->CurrentFrame)
	{
		case 0:
			animation->Frame.X = 0.0f;
		break;

		case 1:
			animation->Frame.X = 19.0f;
		break;
	}

	animation->Frame.Y = 0.0f;

	drawTexture(&animation->AnimationTexture, x, y, &animation->Frame, 0.0f, scale);

	float frameTime = (SDL_GetTicks() - animation->FrameStartTime) / 1000.0f;

	if(frameTime >= animation->AnimationSpeed)
	{
		animation->CurrentFrame++;
		animation->FrameStartTime = SDL_GetTicks();
	}

	if(animation->CurrentFrame == animation->TotalFrames)
	{
		animation->CurrentFrame = 0;
	}
}
