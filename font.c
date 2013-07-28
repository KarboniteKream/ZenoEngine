#include "font.h"

void initFont(Font *font)
{
	initTexture(&font->FontTexture);
	font->FontClips = NULL;
}

void loadFont(Font *font, const char* filename)
{
	// NOTE: Instead of calling this function multiple times, I can load the list of fonts from a file.
	FILE *fontFile = fopen(filename, "rb");

	if(fontFile != NULL)
	{
		char textureFilename[256] = {'\0'};
		uint8_t filenameLength, charHeight, nNums;
		uint8_t flags[32];

		fread(&filenameLength, sizeof(filenameLength), 1, fontFile);
		fread(textureFilename, sizeof(char), filenameLength, fontFile);
		loadTexture(&font->FontTexture, textureFilename);

		fread(&charHeight, sizeof(charHeight), 1, fontFile);
		fread(&nNums, sizeof(nNums), 1, fontFile);
		fread(flags, sizeof(uint8_t), (nNums + 7) / 8, fontFile);

		if(font->FontClips != NULL)
		{
			free(font->FontClips);
		}

		font->FontClips = (RectangleF *)malloc(96 * sizeof(RectangleF));

		for(int i = 0, j = 0; i < nNums; i++)
		{
			int number = 0;
			fread(&number, flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, fontFile);

			if(i % 2 == 0)
			{
				font->FontClips[i / 2].X = number;
				font->FontClips[i / 2].Y = j;
			}
			else
			{
				font->FontClips[i / 2].W = number;
				font->FontClips[i / 2].H = charHeight;

				// NOTE: Could this be simplified?
				if(i % 24 == 23)
				{
					j += charHeight;
				}
			}
		}

		fclose(fontFile);
	}
}

void drawText(Font *font, GLfloat x, GLfloat y, const char* text)
{
	for(unsigned int i = 0; i < strlen(text); i++)
	{
		drawTexture(&font->FontTexture, x, y, &font->FontClips[text[i] - 32], 0.0f);
		x += font->FontClips[text[i] - 32].W;
	}
}
