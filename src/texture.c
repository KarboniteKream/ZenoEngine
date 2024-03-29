#include "texture.h"

void initTexture(Texture *texture)
{
	texture->ID = 0;
	texture->VBO = 0;

	texture->ShaderProgram = 0;

	texture->Width = 0;
	texture->Height = 0;
	texture->TexWidth = 0;
	texture->TexHeight = 0;
}

void initStaticVBO(Texture *texture, VertexData *vertexData, GLuint num)
{
	glGenBuffers(1, &texture->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * num * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void loadTexture(Texture *texture, const char *filename, GLuint shaderProgram)
{
	printLog(0, "Loading texture", filename);

	if(texture->ID != 0)
	{
		glDeleteTextures(1, &texture->ID);
		glDeleteBuffers(1, &texture->VBO);

		texture->ID = 0;
		texture->VBO = 0;
	}

	SDL_Surface *image = IMG_Load(filename);

	if(image == NULL)
	{
		// TODO: Replace with printLog() and load a placeholder texture instead.
		fprintf(stderr, "An error has occurred while loading texture '%s' using SDL_image.\n", filename);
		printLog(1, "SDL error", filename);
		exit(1);
	}

	texture->Width = image->w;
	texture->Height = image->h;

	texture->TexWidth = nextPOT(texture->Width);
	texture->TexHeight = nextPOT(texture->Height);

	glGenTextures(1, &texture->ID);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	if(texture->Width != texture->TexWidth || texture->Height != texture->TexHeight)
	{
		// TODO: Support for other PNG formats.
		// TODO: Handle endianness.
		SDL_Surface *paddedImage = SDL_CreateRGBSurface(0, texture->TexWidth, texture->TexHeight, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

		SDL_SetSurfaceAlphaMod(image, 255);
		SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_NONE);
		SDL_BlitSurface(image, NULL, paddedImage, NULL);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->TexWidth, texture->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, paddedImage->pixels);

		SDL_FreeSurface(paddedImage);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->Width, texture->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(image);

	if(shaderProgram > 0)
	{
		texture->ShaderProgram = shaderProgram;
	}

	if(glGetError() != GL_NO_ERROR)
	{
		// TODO: Call printLog().
		fprintf(stderr, "An error has occurred while loading texture '%s' using OpenGL.\n", filename);
		printLog(1, "OpenGL error", filename);
		exit(1);
	}

	printLog(0, "OK!", filename);
}

// NOTE: Should I rename clip to rectangle?
// NOTE: Should clips be moved into texture itself?
// NOTE: Angle can be calculated using atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
void drawTexture(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat angle, GLfloat scale, bool flip)
{
	if(texture->ID != 0)
	{
		GLfloat texWidth, texHeight;
		GLfloat texLeft, texRight, texTop, texBottom;

		if(clip == NULL)
		{
			texWidth = texture->Width * scale;
			texHeight = texture->Height * scale;

			texLeft = 0.0f;
			texRight = (GLfloat)texture->Width / texture->TexWidth;
			texTop = 0.0f;
			texBottom = (GLfloat)texture->Height / texture->TexHeight;
		}
		else
		{
			texWidth = clip->W * scale;
			texHeight = clip->H * scale;

			texLeft = (clip->X * scale) / (texture->TexWidth * scale);
			texRight = ((clip->X * scale) + texWidth) / (texture->TexWidth * scale);
			texTop = (clip->Y * scale) / (texture->TexHeight * scale);
			texBottom = ((clip->Y * scale) + texHeight) / (texture->TexHeight * scale);
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

		if(flip == true)
		{
			glPushMatrix();
			glTranslatef(x + texWidth / 2, y + texHeight / 2, 0.0f);
			glScalef(-1.0f, 1.0f, 1.0f);
			glTranslatef(-(x + texWidth / 2), -(y + texHeight / 2), 0.0f);
		}

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

		if(angle != 0.0f)
		{
			glPopMatrix();
		}

		if(flip == true)
		{
			glPopMatrix();
		}

		glDisable(GL_TEXTURE_2D);
	}
}

// NOTE: Is this function even needed?
void drawTextureVBO(Texture *texture, GLfloat x, GLfloat y, RectangleF *clip, GLfloat scale)
{
	if(texture->ID != 0)
	{
		GLfloat texWidth, texHeight;
		GLfloat texLeft, texRight, texTop, texBottom;

		if(clip == NULL)
		{
			texWidth = texture->Width * scale;
			texHeight = texture->Height * scale;

			texLeft = 0.0f;
			texRight = (GLfloat)texture->Width / texture->TexWidth;
			texTop = 0.0f;
			texBottom = (GLfloat)texture->Height / texture->TexHeight;
		}
		else
		{
			texWidth = clip->W * scale;
			texHeight = clip->H * scale;

			texLeft = (clip->X * scale) / (texture->TexWidth * scale);
			texRight = ((clip->X * scale) + texWidth) / (texture->TexWidth * scale);
			texTop = (clip->Y * scale) / (texture->TexHeight * scale);
			texBottom = ((clip->Y * scale) + texHeight) / (texture->TexHeight * scale);
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

		if(BOUND_TEXTURE != texture->ID)
		{
			glBindTexture(GL_TEXTURE_2D, texture->ID);
			BOUND_TEXTURE = texture->ID;
		}

		glUseProgram(texShader);

		glEnableVertexAttribArray(texPos);
		glEnableVertexAttribArray(texCoords);

		glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData), vertexData);
		glVertexAttribPointer(texPos, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
		glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
		glDrawArrays(GL_QUADS, 0, 4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(texCoords);
		glDisableVertexAttribArray(texPos);

		glUseProgram(0);
	}
}

void drawTextureWithVBO(Texture *texture, VertexData **vertexData, GLsizei num, uint32_t rgb_color)
{
	if(BOUND_TEXTURE != texture->ID)
	{
		glBindTexture(GL_TEXTURE_2D, texture->ID);
		BOUND_TEXTURE = texture->ID;
	}

	glUseProgram(texture->ShaderProgram);
	glUniform4f(texColor, ((rgb_color >> 16) & 0xFF) / 255.0, ((rgb_color >> 8) & 0xFF) / 255.0, (rgb_color & 0xFF) / 255.0, 1.0f);

	glEnableVertexAttribArray(texPos);
	glEnableVertexAttribArray(texCoords);

	glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(VertexData), *vertexData, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(texPos, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
	glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
	glDrawArrays(GL_QUADS, 0, num);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(texCoords);
	glDisableVertexAttribArray(texPos);

	glUseProgram(0);
}

// NOTE: If this function is only used in level, move it there.
void drawTextureWithStaticVBO(Texture *texture, GLsizei num)
{
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if(BOUND_TEXTURE != texture->ID)
	{
		glBindTexture(GL_TEXTURE_2D, texture->ID);
		BOUND_TEXTURE = texture->ID;
	}

	glBindBuffer(GL_ARRAY_BUFFER, texture->VBO);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
	glVertexPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
	glDrawArrays(GL_QUADS, 0, num);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
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
