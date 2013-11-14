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

		fclose(fontFile);

		glGenBuffers(1, &font->FontTexture.VBO);
	}
}

void drawText(Font *font, GLfloat x, GLfloat y, const char* text, GLfloat r, GLfloat g, GLfloat b)
{
	VertexData *vertexData = (VertexData *)malloc(4 * strlen(text) * sizeof(VertexData));

	int num = 0;
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
		else if(text[i] == ' ')
		{
			x += font->Clips[0].W;
		}
		else
		{
			vertexData[num].X = x;
			vertexData[num].Y = y;
			vertexData[num].S = font->Clips[text[i] - 32].X / font->FontTexture.TexWidth;
			vertexData[num].T = font->Clips[text[i] - 32].Y / font->FontTexture.TexHeight;

			vertexData[num + 1].X = x + font->Clips[text[i] - 32].W;
			vertexData[num + 1].Y = y;
			vertexData[num + 1].S = (font->Clips[text[i] - 32].X + font->Clips[text[i] - 32].W) / font->FontTexture.TexWidth;
			vertexData[num + 1].T = font->Clips[text[i] - 32].Y / font->FontTexture.TexHeight;

			vertexData[num + 2].X = x + font->Clips[text[i] - 32].W;
			vertexData[num + 2].Y = y + font->Clips[text[i] - 32].H;
			vertexData[num + 2].S = (font->Clips[text[i] - 32].X + font->Clips[text[i] - 32].W) / font->FontTexture.TexWidth;
			vertexData[num + 2].T = (font->Clips[text[i] - 32].Y + font->Clips[text[i] - 32].H) / font->FontTexture.TexHeight;

			vertexData[num + 3].X = x;
			vertexData[num + 3].Y = y + font->Clips[text[i] - 32].H;
			vertexData[num + 3].S = font->Clips[text[i] - 32].X / font->FontTexture.TexWidth;
			vertexData[num + 3].T = (font->Clips[text[i] - 32].Y + font->Clips[text[i] - 32].H) / font->FontTexture.TexHeight;

			x += font->Clips[text[i] - 32].W;
			num += 4;
		}
	}

	drawTextureWithVBO(&font->FontTexture, &vertexData, num, r, g, b);

	free(vertexData);
}
