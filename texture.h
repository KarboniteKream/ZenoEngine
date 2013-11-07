#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"
#include "lodepng.h"

typedef struct
{
	GLuint ID;
	GLuint VBO;

	GLuint Width;
	GLuint Height;
	GLuint TexWidth;
	GLuint TexHeight;
} Texture;

void initTexture(Texture *texture);
void initVBO(Texture *texture, GLuint num);
void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint num);
void loadTexture(Texture *texture, const char *filename);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle, GLfloat scale);
void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle);
GLuint nextPOT(GLuint number);

#endif
