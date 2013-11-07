#include "texture.h"

void initTexture(Texture *texture)
{
	texture->ID = 0;
	texture->VBO = 0;

	texture->Width = 0;
	texture->Height = 0;
	texture->TexWidth = 0;
	texture->TexHeight = 0;
}

void initVBO(Texture *texture, GLuint num)
{
	VertexData vertexData[num];

	glGenBuffers(1, &texture->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(VertexData), vertexData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint num)
{
	glGenBuffers(1, &texture->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * num * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void loadTexture(Texture *texture, const char *filename)
{
	if(texture->ID != 0)
	{
		glDeleteTextures(1, &texture->ID);
		glDeleteBuffers(1, &texture->VBO);

		texture->ID = 0;
		texture->VBO = 0;
	}

	unsigned char *image = NULL;

	if(lodepng_decode32_file(&image, &texture->Width, &texture->Height, filename) > 0)
	{
		fprintf(stderr, "An error has occurred while loading texture '%s' using LodePNG.\n", filename);
		exit(1);
	}

	texture->TexWidth = nextPOT(texture->Width);
	texture->TexHeight = nextPOT(texture->Height);

	glGenTextures(1, &texture->ID);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	if(texture->Width != texture->TexWidth || texture->Height != texture->TexHeight)
	{
		unsigned char *paddedImage = (unsigned char *)calloc(texture->TexWidth * texture->TexHeight * 4, sizeof(unsigned char));

		for(int i = 0; i < texture->TexWidth; i++)
		{
			for(int j = 0; j < texture->TexHeight; j++)
			{
				for(int k = 0; k < 4; k++)
				{
					paddedImage[4 * texture->TexWidth * j + 4 * i + k] = image[4 * texture->Width * j + 4 * i + k];
				}
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->TexWidth, texture->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &paddedImage[0]);
		free(paddedImage);
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
// NOTE: Should clips be moved into texture itself?
void drawTexture(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle, GLfloat scale)
{
	if(texture->ID != 0)
	{
		GLfloat texWidth = texture->Width;
		GLfloat texHeight = texture->Height;

		GLfloat texLeft = 0.0f;
		GLfloat texRight = (GLfloat)texture->Width / texture->TexWidth;
		GLfloat texTop = 0.0f;
		GLfloat texBottom = (GLfloat)texture->Height / texture->TexHeight;

		if(clip != NULL)
		{
			texWidth = clip->W;
			texHeight = clip->H;

			texLeft = clip->X / texture->TexWidth;
			texRight = (clip->X + texWidth) / texture->TexWidth;
			texTop = clip->Y / texture->TexHeight;
			texBottom = (clip->Y + texHeight) / texture->TexHeight;
		}

		// TODO: Call this only once per frame.
		glEnable(GL_TEXTURE_2D);

		if(BOUND_TEXTURE != texture->ID)
		{
			glBindTexture(GL_TEXTURE_2D, texture->ID);
			BOUND_TEXTURE = texture->ID;
		}

		if(angle != 0.0f)
		{
			glPushMatrix();

			glTranslatef(x + texWidth / 2, y + texHeight / 2, 0.0f);
			glRotatef(angle, 0.0f, 0.0f, 1.0f);
			glTranslatef(-(x + texWidth / 2), -(y + texHeight / 2), 0.0f);
		}

		glScalef(scale, scale, scale);

		glBegin(GL_QUADS);
			glTexCoord2f(texLeft, texTop);
			glVertex2f(x, y);
			glTexCoord2f(texRight, texTop);
			glVertex2f(x + texWidth, y);
			glTexCoord2f(texRight, texBottom);
			glVertex2f(x + texWidth, y + texHeight);
			glTexCoord2f(texLeft, texBottom);
			glVertex2f(x, y + texHeight);
		glEnd();

		glScalef(1.0f, 1.0f, 1.0f);

		if(angle != 0.0f)
		{
			glPopMatrix();
		}

		glDisable(GL_TEXTURE_2D);
	}
}

void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, Rectangle *clip, GLfloat angle)
{
	if(texture->ID != 0)
	{
		GLfloat texWidth = texture->Width;
		GLfloat texHeight = texture->Height;

		GLfloat texLeft = 0.0f;
		GLfloat texRight = (GLfloat)texture->Width / texture->TexWidth;
		GLfloat texTop = 0.0f;
		GLfloat texBottom = (GLfloat)texture->Height / texture->TexHeight;

		if(clip != NULL)
		{
			texWidth = clip->W;
			texHeight = clip->H;

			texLeft = clip->X / texture->TexWidth;
			texRight = (clip->X + texWidth) / texture->TexWidth;
			texTop = clip->Y / texture->TexHeight;
			texBottom = (clip->Y + texHeight) / texture->TexHeight;
		}

		VertexData vertexData[4];

		vertexData[0].X = x;
		vertexData[0].Y = y;
		vertexData[0].S = texLeft;
		vertexData[0].T = texTop;

		vertexData[1].X = x + texWidth;
		vertexData[1].Y = y;
		vertexData[1].S = texRight;
		vertexData[1].T = texTop;

		vertexData[2].X = x + texWidth;
		vertexData[2].Y = y + texHeight;
		vertexData[2].S = texRight;
		vertexData[2].T = texBottom;

		vertexData[3].X = x;
		vertexData[3].Y = y + texHeight;
		vertexData[3].S = texLeft;
		vertexData[3].T = texBottom;

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_TEXTURE_2D);

		if(BOUND_TEXTURE != texture->ID)
		{
			glBindTexture(GL_TEXTURE_2D, texture->ID);
			BOUND_TEXTURE = texture->ID;
		}

		glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData), vertexData);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
		glVertexPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
		glDrawArrays(GL_QUADS, 0, 4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisable(GL_TEXTURE_2D);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
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
