#ifndef ENTITY_H
#define ENTITY_H

#include <math.h>

#include "globals.h"
// NOTE: Only for debugging.
#include "util.h"
#include "texture.h"
#include "level.h"

typedef struct
{
	Texture EntityTextures[2];
	RectangleF BoundingBox;

	GLfloat X;
	GLfloat Y;

	GLfloat MaxAngle;
	GLfloat Angle;
	GLfloat AngleSpeed;

	int Health;

	int i;
} Entity;

// NOTE: Should this be combined?
void initEntity(Entity *entity);
void loadEntity(Entity *entity, const char *textureFilename, GLfloat x, GLfloat y);

// TODO: Rename.
void startEntity(Entity *entity);
void updateEntity(Entity *entity);

void drawEntity(Entity *entity);

#endif
