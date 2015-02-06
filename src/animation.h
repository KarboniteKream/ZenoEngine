#ifndef ANIMATION_H
#define ANIMATION_H

#include "globals.h"
#include "texture.h"

typedef struct
{
	Texture AnimationTexture;

	int TotalFrames;
	int FramesPerRow;
	int FramesPerColumn;

	int CurrentFrame;
	float AnimationSpeed;

	bool Started;
	uint32_t FrameStartTime;

	bool IsFinished;
	bool IsLastFrame;
	bool IsSkippable;

	RectangleF Frame;

	GLfloat X;
	GLfloat Y;
	GLfloat Scale;
	bool Flip;
} Animation;

void initAnimation(Animation *animation);
void loadAnimation(Animation *animation, const char *animationTexture, int totalFrames, int columns, int rows, float animationSpeed, bool isSkippable);
void playAnimation(Animation *animation, GLfloat x, GLfloat y, GLfloat scale, bool flip);
void playStaticAnimation(Animation *animation);
void startAnimation(Animation *animation, GLfloat x, GLfloat y, GLfloat scale, bool flip);
void stopAnimation(Animation *animation);

#endif
