#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"

typedef struct
{
	GLuint ID;
	GLuint VBO;

	GLuint ShaderProgram;

	GLuint Width;
	GLuint Height;
	GLuint TexWidth;
	GLuint TexHeight;
} Texture;

void initTexture(Texture *texture);
void initVBO(Texture *texture, GLuint num);
void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint num);
void loadTexture(Texture *texture, const char *filename);
void loadTextureShader(Texture *texture, GLuint shaderProgram);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat angle, GLfloat scale);
void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat scale);
GLuint nextPOT(GLuint number);

#endif
