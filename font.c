#include "font.h"

void initFont(Font *font, uint8_t num)
{
	font->Num = num;

	font->Textures = (Texture *)malloc(font->Num * sizeof(Texture));
	font->Clips = (RectangleF **)malloc(font->Num * sizeof(RectangleF *));
	font->Height = (uint8_t *)malloc(font->Num * sizeof(uint8_t));

	for(uint8_t i = 0; i < font->Num; i++)
	{
		initTexture(&font->Textures[i]);
		font->Clips[i] = NULL;
		font->Height[i] = 0;
	}
}

void loadFont(Font *font, uint8_t index, const char* filename, GLuint shaderProgram)
{
	printLog(0, "Loading font", filename);

	// NOTE: Instead of calling this function multiple times, I can load the list of fonts from a file.
	FILE *fontFile = fopen(filename, "rb");

	if(fontFile != NULL)
	{
		printLog(0, "File successfully opened.", filename);

		char textureFilename[256] = {'\0'};
		uint8_t filenameLength, num;
		uint8_t flags[32];

		// TODO: Replace sizeof() with actual size of data in file.
		fread(&filenameLength, sizeof(filenameLength), 1, fontFile);
		fread(textureFilename, sizeof(char), filenameLength, fontFile);
		loadTexture(&font->Textures[index], textureFilename, shaderProgram);

		fread(&font->Height[index], sizeof(uint8_t), 1, fontFile);
		fread(&num, sizeof(num), 1, fontFile);
		fread(flags, sizeof(uint8_t), (num + 7) / 8, fontFile);

		if(font->Clips[index] != NULL)
		{
			// NOTE: loadTexture() automatically frees the texture.
			free(font->Clips[index]);
		}

		font->Clips[index] = (RectangleF *)malloc(96 * sizeof(RectangleF));

		for(int i = 0, j = 0; i < num; i++)
		{
			int number = 0;
			fread(&number, flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, fontFile);

			if(i % 2 == 0)
			{
				font->Clips[index][i / 2].X = number;
				font->Clips[index][i / 2].Y = j;
			}
			else
			{
				font->Clips[index][i / 2].W = number;
				font->Clips[index][i / 2].H = font->Height[index];

				// NOTE: Could this be simplified?
				if(i % 24 == 23)
				{
					j += font->Height[index];
				}
			}
		}

		fclose(fontFile);

		glGenBuffers(1, &font->Textures[index].VBO);

		printLog(0, "OK!", filename);
	}
}

void drawText(Font *font, uint8_t index, GLfloat x, GLfloat y, const char* text, uint32_t color)
{
	// TODO: Compare index to font->Num, use fallback font.
	VertexData *vertexData = (VertexData *)malloc(4 * strlen(text) * sizeof(VertexData));

	int num = 0;
	GLfloat startX = x;

	// TODO: Regenerate data only when necessary.
	for(unsigned int i = 0; i < strlen(text); i++)
	{
		if(text[i] == '\n')
		{
			x = startX;
			y += font->Height[index];
		}
		else if(text[i] == '\t')
		{
			x += 4 * font->Clips[index][0].W;
		}
		else if(text[i] == ' ')
		{
			x += font->Clips[index][0].W;
		}
		else
		{
			vertexData[num].X = x;
			vertexData[num].Y = y;
			vertexData[num].S = font->Clips[index][text[i] - 32].X / font->Textures[index].TexWidth;
			vertexData[num].T = font->Clips[index][text[i] - 32].Y / font->Textures[index].TexHeight;

			vertexData[num + 1].X = x + font->Clips[index][text[i] - 32].W;
			vertexData[num + 1].Y = y;
			vertexData[num + 1].S = (font->Clips[index][text[i] - 32].X + font->Clips[index][text[i] - 32].W) / font->Textures[index].TexWidth;
			vertexData[num + 1].T = font->Clips[index][text[i] - 32].Y / font->Textures[index].TexHeight;

			vertexData[num + 2].X = x + font->Clips[index][text[i] - 32].W;
			vertexData[num + 2].Y = y + font->Clips[index][text[i] - 32].H;
			vertexData[num + 2].S = (font->Clips[index][text[i] - 32].X + font->Clips[index][text[i] - 32].W) / font->Textures[index].TexWidth;
			vertexData[num + 2].T = (font->Clips[index][text[i] - 32].Y + font->Clips[index][text[i] - 32].H) / font->Textures[index].TexHeight;

			vertexData[num + 3].X = x;
			vertexData[num + 3].Y = y + font->Clips[index][text[i] - 32].H;
			vertexData[num + 3].S = font->Clips[index][text[i] - 32].X / font->Textures[index].TexWidth;
			vertexData[num + 3].T = (font->Clips[index][text[i] - 32].Y + font->Clips[index][text[i] - 32].H) / font->Textures[index].TexHeight;

			x += font->Clips[index][text[i] - 32].W;
			num += 4;
		}
	}

	drawTextureWithVBO(&font->Textures[index], &vertexData, num, color);

	free(vertexData);
}
