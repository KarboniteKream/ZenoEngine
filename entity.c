#include "entity.h"

void initEntity(Entity *entity)
{
	initTexture(&entity->EntityTextures[0]);
	initTexture(&entity->EntityTextures[1]);

	entity->BoundingBox.X = 0.0f;
	entity->BoundingBox.Y = 0.0f;
	entity->BoundingBox.W = 0.0f;
	entity->BoundingBox.H = 0.0f;

	entity->MaxAngle = 0.0f;
	entity->Angle = 0.0f;
	entity->AngleSpeed = 0.0f;

	entity->X = 0.0f;
	entity->Y = 0.0f;

	entity->State = 0;
	entity->Health = 0;
}

void loadEntity(Entity *entity, const char *textureFilename, GLfloat x, GLfloat y)
{
	loadTexture(&entity->EntityTextures[0], textureFilename);
	loadTexture(&entity->EntityTextures[1], "images/scarecrowDamaged.png");

	// NOTE: Is this bad practice?
	entity->BoundingBox = (RectangleF){0.0f, 0.0f, entity->EntityTextures[0].Width, entity->EntityTextures[0].Height};

	entity->MaxAngle = 22.5f;
	entity->Angle = 0.0f;
	entity->AngleSpeed = 5.0f;

	entity->X = x;
	entity->Y = y;

	entity->State = 0;
	entity->Health = 100;
}

void startEntity(Entity *entity)
{
	entity->AngleSpeed = 5.0f;
	entity->MaxAngle = 22.5f;

	if(entity->State == 0)
	{
		entity->State = 1;
	}
	else
	{
		entity->State = 0;
	}
}

void updateEntity(Entity *entity)
{
	if(entity->AngleSpeed > 0.0f)
	{
		entity->Angle += entity->AngleSpeed * DELTA_TICKS;
		entity->AngleSpeed -= 1.0f * DELTA_TICKS;

		if(entity->Angle > 6.28f)
		{
			entity->Angle = 0.0f;
		}
	}
	else
	{
		entity->Angle = 0.0f;
	}
}

void drawEntity(Entity *entity)
{
	glPushMatrix();

	// NOTE: We don't rotate around center.
	glTranslatef(entity->X + entity->EntityTextures[0].Width / 2 * 4, entity->Y + entity->EntityTextures[0].Height * 4, 0.0f);
	glRotatef(entity->MaxAngle * sin(entity->Angle), 0.0f, 0.0f, 1.0f);
	glTranslatef(-(entity->X + entity->EntityTextures[0].Width / 2 * 4), -(entity->Y + entity->EntityTextures[0].Height * 4), 0.0f);

	drawTexture(&entity->EntityTextures[entity->State], entity->X, entity->Y, NULL, 0.0f, 4.0f, false);

	if(DEBUG == true)
	{
		drawRectangle(entity->X + entity->BoundingBox.X * 4, entity->Y + entity->BoundingBox.Y * 4, entity->BoundingBox.W * 4, entity->BoundingBox.H * 4, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	glPopMatrix();
}
