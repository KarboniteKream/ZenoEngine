#include "texture.h"

void initTexture(Texture *texture)
{
	texture->ID = 0;
	texture->VBO = 0;
	texture->IBO = 0;

	texture->ImageWidth = 0;
	texture->ImageHeight = 0;
	texture->TexWidth = 0;
	texture->TexHeight = 0;
}

void loadTexture(Texture *texture, const char *filename)
{
	if(texture->ID != 0)
	{
		glDeleteTextures(1, &texture->ID);
		texture->ID = 0;
	}

	unsigned char *image = NULL;

	if(lodepng_decode32_file(&image, &texture->ImageWidth, &texture->ImageHeight, filename) > 0)
	{
		fprintf(stderr, "An error has occurred while loading texture '%s' using LodePNG.\n", filename);
		exit(1);
	}

	texture->TexWidth = nextPOT(texture->ImageWidth);
	texture->TexHeight = nextPOT(texture->ImageHeight);

	glGenTextures(1, &texture->ID);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	if(texture->ImageWidth != texture->TexWidth || texture->ImageHeight != texture->TexHeight)
	{
		unsigned char paddedImage[texture->TexWidth * texture->TexHeight * 4];

		for(int i = 0; i < texture->TexWidth; i++)
		{
			for(int j = 0; j < texture->TexHeight; j++)
			{
				for(int k = 0; k < 4; k++)
				{
					paddedImage[4 * texture->TexWidth * j + 4 * i + k] = image[4 * texture->ImageWidth * j + 4 * i + k];
				}
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->TexWidth, texture->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &paddedImage[0]);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->TexWidth, texture->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
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
void drawTexture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle)
{
	if(texture->ID != 0)
	{
		GLfloat TexWidth = texture->ImageWidth;
		GLfloat TexHeight = texture->ImageHeight;

		GLfloat TexLeft = 0.0f;
		GLfloat TexRight = (GLfloat)texture->ImageWidth / texture->TexWidth;
		GLfloat TexTop = 0.0f;
		GLfloat TexBottom = (GLfloat)texture->ImageHeight / texture->TexHeight;

		if(clip != NULL)
		{
			TexWidth = clip->W;
			TexHeight = clip->H;

			TexLeft = clip->X / texture->TexWidth;
			TexRight = (clip->X + TexWidth) / texture->TexWidth;
			TexTop = clip->Y / texture->TexHeight;
			TexBottom = (clip->Y + TexHeight) / texture->TexHeight;
		}

		glEnable(GL_TEXTURE_2D);

		if(BOUND_TEXTURE != texture->ID)
		{
			glBindTexture(GL_TEXTURE_2D, texture->ID);
			BOUND_TEXTURE = texture->ID;
		}

		if(angle != 0.0f)
		{
			glPushMatrix();

			glTranslatef(x + TexWidth / 2, y + TexHeight / 2, 0.0f);
			glRotatef(angle, 0.0f, 0.0f, 1.0f);
			glTranslatef(-(x + TexWidth / 2), -(y + TexHeight / 2), 0.0f);
		}

		glBegin(GL_QUADS);
			glTexCoord2f(TexLeft, TexTop);
			glVertex2f(x, y);
			glTexCoord2f(TexRight, TexTop);
			glVertex2f(x + TexWidth, y);
			glTexCoord2f(TexRight, TexBottom);
			glVertex2f(x + TexWidth, y + TexHeight);
			glTexCoord2f(TexLeft, TexBottom);
			glVertex2f(x, y + TexHeight);
		glEnd();

		if(angle != 0.0f)
		{
			glPopMatrix();
		}

		glDisable(GL_TEXTURE_2D);
	}
}

GLuint nextPOT(GLuint number)
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
