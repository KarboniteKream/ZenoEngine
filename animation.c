#include "animation.h"

void initAnimation(Animation *animation)
{
	initTexture(&animation->AnimationTexture);

	animation->TotalFrames = 0;
	animation->FramesPerRow = 0;
	animation->FramesPerColumn = 0;

	animation->CurrentFrame = 0;
	animation->AnimationSpeed = 0.0f;

	animation->Started = false;
	animation->FrameStartTime = 0;

	animation->IsFinished = false;
	animation->IsLastFrame = false;

	animation->Frame.X = 0.0f;
	animation->Frame.Y = 0.0f;
	animation->Frame.W = 0.0f;
	animation->Frame.H = 0.0f;
}

void loadAnimation(Animation *animation, const char *animationTexture, int totalFrames, int framesPerRow, int framesPerColumn, float animationSpeed)
{
	loadTexture(&animation->AnimationTexture, animationTexture);

	animation->TotalFrames = totalFrames;
	animation->FramesPerRow = framesPerRow;
	animation->FramesPerColumn = framesPerColumn;

	animation->CurrentFrame = 0;
	animation->AnimationSpeed = animationSpeed;

	animation->Started = false;
	animation->FrameStartTime = 0;

	animation->IsFinished = false;
	animation->IsLastFrame = false;

	animation->Frame.X = 0.0f;
	animation->Frame.Y = 0.0f;
	animation->Frame.W = animation->AnimationTexture.Width / animation->FramesPerRow;
	animation->Frame.H = animation->AnimationTexture.Height / animation->FramesPerColumn;
}

void playAnimation(Animation *animation, GLfloat x, GLfloat y, GLfloat scale, bool flip)
{
	if(animation->Started == false)
	{
		animation->FrameStartTime = SDL_GetTicks();
		animation->Started = true;
	}

	// TODO: Store in struct instead of calculating every frame.
	animation->Frame.X = (animation->CurrentFrame % animation->FramesPerRow) * animation->Frame.W;
	animation->Frame.Y = (int)(animation->CurrentFrame / animation->FramesPerRow) * animation->Frame.H;

	drawTexture(&animation->AnimationTexture, x, y, &animation->Frame, 0.0f, scale, flip);

	float frameTime = (SDL_GetTicks() - animation->FrameStartTime) / 1000.0f;

	if(frameTime >= animation->AnimationSpeed)
	{
		animation->CurrentFrame++;
		animation->FrameStartTime = SDL_GetTicks();
	}

	if(animation->CurrentFrame == animation->TotalFrames)
	{
		animation->CurrentFrame = 0;
		animation->IsLastFrame = true;
	}

	if(animation->CurrentFrame == 0 && animation->IsLastFrame == true)
	{
		animation->IsFinished = true;
		animation->IsLastFrame = false;
	}
}

void playStaticAnimation(Animation* animation)
{
	if(animation->Started == false)
	{
		animation->FrameStartTime = SDL_GetTicks();
		animation->Started = true;
	}

	// TODO: Store in struct instead of calculating every frame.
	animation->Frame.X = (animation->CurrentFrame % animation->FramesPerRow) * animation->Frame.W;
	animation->Frame.Y = (int)(animation->CurrentFrame / animation->FramesPerRow) * animation->Frame.H;

	drawTexture(&animation->AnimationTexture, animation->X, animation->Y, &animation->Frame, 0.0f, animation->Scale, animation->Flip);

	float frameTime = (SDL_GetTicks() - animation->FrameStartTime) / 1000.0f;

	if(frameTime >= animation->AnimationSpeed)
	{
		animation->CurrentFrame++;
		animation->FrameStartTime = SDL_GetTicks();
	}

	if(animation->CurrentFrame == animation->TotalFrames)
	{
		animation->CurrentFrame = 0;
		animation->IsLastFrame = true;
	}

	if(animation->CurrentFrame == 0 && animation->IsLastFrame == true)
	{
		animation->IsFinished = true;
		animation->IsLastFrame = false;
	}
}

void startAnimation(Animation *animation, GLfloat x, GLfloat y, GLfloat scale, bool flip)
{
	animation->Started = true;

	animation->X = x;
	animation->Y = y;
	animation->Scale = scale;
	animation->Flip = flip;
}

void stopAnimation(Animation *animation)
{
	animation->Started = false;
	animation->CurrentFrame = 0;

	animation->IsFinished = false;
	animation->IsLastFrame = false;
}
