#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"

typedef struct
{
	// NOTE: Should I add coordinates?
	GLuint ID;
	GLuint VBO;

	GLuint ShaderProgram;

	GLuint Width;
	GLuint Height;
	GLuint TexWidth;
	GLuint TexHeight;
} Texture;

// TODO: Implement freeTexture().
void initTexture(Texture *texture);
void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint num);
void loadTexture(Texture *texture, const char *filename, GLuint shaderProgram);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat angle, GLfloat scale, bool flip);
void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat scale);
void drawTextureWithVBO(Texture *texture, VertexData **vertexData, GLsizei num, uint32_t color);
void drawTextureWithStaticVBO(Texture *texture, GLsizei num);
GLuint nextPOT(GLuint number);

#endif
