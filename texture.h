#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"
#include "lodepng.h"

typedef struct
{
	GLuint TextureID;

	GLuint ImageWidth;
	GLuint ImageHeight;
	GLuint TexWidth;
	GLuint TexHeight;
} Texture;

void initTexture(Texture *texture);
void loadTexture(Texture *texture, const char *filename);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat angle);
GLuint nextPOT(GLuint number);

#endif
