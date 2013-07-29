#include "texture.h"

void init_texture(Texture *texture)
{
	texture->texture_ID = 0;

	texture->image_width = 0;
	texture->image_height = 0;
	texture->tex_width = 0;
	texture->tex_height = 0;
}

void load_texture(Texture *texture, const char *filename)
{
	if(texture->texture_ID != 0)
	{
		glDeleteTextures(1, &texture->texture_ID);
		texture->texture_ID = 0;
	}

	unsigned char *image = NULL;

	if(lodepng_decode32_file(&image, &texture->image_width, &texture->image_height, filename) > 0)
	{
		fprintf(stderr, "An error has occurred while loading texture '%s' using LodePNG.\n", filename);
		exit(1);
	}

	texture->tex_width = next_POT(texture->image_width);
	texture->tex_height = next_POT(texture->image_height);

	glGenTextures(1, &texture->texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture->texture_ID);

	if(texture->image_width != texture->tex_width || texture->image_height != texture->tex_height)
	{
		unsigned char padded_image[texture->tex_width * texture->tex_height * 4];

		for(int i = 0; i < texture->tex_width; i++)
		{
			for(int j = 0; j < texture->tex_height; j++)
			{
				for(int k = 0; k < 4; k++)
				{
					padded_image[4 * texture->tex_width * j + 4 * i + k] = image[4 * texture->image_width * j + 4 * i + k];
				}
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->tex_width, texture->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &padded_image[0]);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->tex_width, texture->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	free(image);

	if(glGetError() != GL_NO_ERROR)
	{
		fprintf(stderr, "An error has occurred while loading texture '%s' using OpenGL.\n", filename);
		exit(1);
	}
}

// NOTE: Should I rename clip to rectangle?
void draw_texture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle)
{
	if(texture->texture_ID != 0)
	{
		GLfloat tex_width = texture->image_width;
		GLfloat tex_height = texture->image_height;

		GLfloat tex_left = 0.0f;
		GLfloat tex_right = (GLfloat)texture->image_width / texture->tex_width;
		GLfloat tex_top = 0.0f;
		GLfloat tex_bottom = (GLfloat)texture->image_height / texture->tex_height;

		if(clip != NULL)
		{
			tex_width = clip->w;
			tex_height = clip->h;

			tex_left = clip->x / texture->tex_width;
			tex_right = (clip->x + tex_width) / texture->tex_width;
			tex_top = clip->y / texture->tex_height;
			tex_bottom = (clip->y + tex_height) / texture->tex_height;
		}

		glEnable(GL_TEXTURE_2D);

		if(BOUND_TEXTURE != texture->texture_ID)
		{
			glBindTexture(GL_TEXTURE_2D, texture->texture_ID);
			BOUND_TEXTURE = texture->texture_ID;
		}

		if(angle != 0.0f)
		{
			glPushMatrix();

			glTranslatef(x + tex_width / 2, y + tex_height / 2, 0.0f);
			glRotatef(angle, 0.0f, 0.0f, 1.0f);
			glTranslatef(-(x + tex_width / 2), -(y + tex_height / 2), 0.0f);
		}

		glBegin(GL_QUADS);
			glTexCoord2f(tex_left, tex_top);
			glVertex2f(x, y);
			glTexCoord2f(tex_right, tex_top);
			glVertex2f(x + tex_width, y);
			glTexCoord2f(tex_right, tex_bottom);
			glVertex2f(x + tex_width, y + tex_height);
			glTexCoord2f(tex_left, tex_bottom);
			glVertex2f(x, y + tex_height);
		glEnd();

		if(angle != 0.0f)
		{
			glPopMatrix();
		}

		glDisable(GL_TEXTURE_2D);
	}
}

GLuint next_POT(GLuint number)
{
	if(number != 0)
	{
		number--;
		number |= (number >> 1);
		number |= (number >> 2);
		number |= (number >> 4);
		number |= (number >> 8);
		number |= (number >> 16);
		number++;
	}

	return number;
}
