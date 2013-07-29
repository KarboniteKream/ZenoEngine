#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"
#include "lodepng.h"

typedef struct
{
	GLuint texture_ID;

	GLuint image_width;
	GLuint image_height;
	GLuint tex_width;
	GLuint tex_height;
} Texture;

void init_texture(Texture *texture);
void load_texture(Texture *texture, const char *filename);
void draw_texture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle);
GLuint next_POT(GLuint number);

#endif
