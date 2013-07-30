#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"
#include "lodepng.h"

typedef struct
{
	GLuint ID;
	GLuint VBO;
	GLuint IBO;

	GLuint ImageWidth;
	GLuint ImageHeight;
	GLuint TexWidth;
	GLuint TexHeight;
} Texture;

void initTexture(Texture *texture);
void loadTexture(Texture *texture, const char *filename);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle);
GLuint nextPOT(GLuint number);

#endif
