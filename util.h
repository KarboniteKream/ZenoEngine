#ifndef UTIL_H
#define UTIL_H

#include "globals.h"

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
} Rectangle;

typedef struct
{
	GLfloat x;
	GLfloat y;
} Coordinate;

// TODO: Add color support.
void draw_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void draw_empty_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat line_width);

#endif
