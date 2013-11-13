#include "font.h"

void initFont(Font *font)
{
	initTexture(&font->FontTexture);
	font->Clips = NULL;

	font->Height = 0;
}

void loadFont(Font *font, const char* filename)
{
	// NOTE: Instead of calling this function multiple times, I can load the list of fonts from a file.
	FILE *fontFile = fopen(filename, "rb");

	if(fontFile != NULL)
	{
		char textureFilename[256] = {'\0'};
		uint8_t filenameLength, num;
		uint8_t flags[32];

		fread(&filenameLength, sizeof(filenameLength), 1, fontFile);
		fread(textureFilename, sizeof(char), filenameLength, fontFile);
		loadTexture(&font->FontTexture, textureFilename);

		fread(&font->Height, sizeof(font->Height), 1, fontFile);
		fread(&num, sizeof(num), 1, fontFile);
		fread(flags, sizeof(uint8_t), (num + 7) / 8, fontFile);

		if(font->Clips != NULL)
		{
			free(font->Clips);
		}

		font->Clips = (RectangleF *)calloc(96, sizeof(RectangleF));

		for(int i = 0, j = 0; i < num; i++)
		{
			int number = 0;
			fread(&number, flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, fontFile);

			if(i % 2 == 0)
			{
				font->Clips[i / 2].X = number;
				font->Clips[i / 2].Y = j;
			}
			else
			{
				font->Clips[i / 2].W = number;
				font->Clips[i / 2].H = font->Height;

				// NOTE: Could this be simplified?
				if(i % 24 == 23)
				{
					j += font->Height;
				}
			}
		}

		initVBO(&font->FontTexture, 4);

		fclose(fontFile);
	}
}

void drawText(Font *font, GLfloat x, GLfloat y, const char* text, GLfloat r, GLfloat g, GLfloat b)
{
	glUseProgram(texShader);
	glUniform4f(texColor, r, g, b, 1.0f);
	glUseProgram(0);

	GLfloat startX = x;

	for(unsigned int i = 0; i < strlen(text); i++)
	{
		if(text[i] == '\n')
		{
			x = startX;
			y += font->Height;
		}
		else if(text[i] == '\t')
		{
			x += 4 * font->Clips[0].W;
		}
		else
		{
			drawTextureVBO(&font->FontTexture, x, y, &font->Clips[text[i] - 32], 0.0f, 1.0f);
			x += font->Clips[text[i] - 32].W;
		}
	}
}
