#ifndef TEXTURE_H
#define TEXTURE_H

#define GL_GLEXT_PROTOTYPES

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
void initVBO(Texture *texture);
void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint *indexData, GLuint num);
void loadTexture(Texture *texture, const char *filename);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle);
void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle);
GLuint nextPOT(GLuint number);

#endif
