#include "level.h"

void initLevel(Level *level)
{
	initTexture(&level->LevelTexture);
	level->TextureClips = NULL;

	level->Width = 0;
	level->Height = 0;

	level->Layout = NULL;
	level->Properties = NULL;

	level->Debug = false;
}

void loadLevel(Level *level, const char *filename)
{
	char levelFilename[256] = {'\0'};
	char textureFilename[256] = {'\0'};

	sprintf(levelFilename, "levels/%s.map", filename);
	// FIXME: What about levels with same textures (regarding filename)?
	// NOTE: Should be resolved with moving metadata.
	sprintf(textureFilename, "images/%s-%d.png", filename, BLOCK_SIZE);

	// Move metadata to .dat files.
	level->Width = 60;
	level->Height = 40;

	if(level->Layout != NULL)
	{
		for(int i = 0; i < level->Width; i++)
		{
			for(int j = 0; j < level->Height; j++)
			{
				free(level->Properties[i][j]);
			}

			free(level->Layout[i]);
			free(level->Properties[i]);
		}

		free(level->TextureClips);
		free(level->Layout);
		free(level->Properties);
	}

	level->Layout = (uint8_t **)malloc(level->Width * sizeof(uint8_t *));
	level->Properties = (uint8_t ***)malloc(level->Width * sizeof(uint8_t **));

	for(int i = 0; i < level->Width; i++)
	{
		level->Layout[i] = (uint8_t *)calloc(level->Height, sizeof(uint8_t));
		level->Properties[i] = (uint8_t **)malloc(level->Height * sizeof(uint8_t *));

		for(int j = 0; j < level->Height; j++)
		{
			level->Properties[i][j] = (uint8_t *)calloc(1, sizeof(uint8_t));
		}
	}

	FILE *levelFile = fopen(levelFilename, "rb");

	if(levelFile != NULL)
	{
		int tile = 0;

		for(int i = 0; i < level->Width; i++)
		{
			for(int j = 0; j < level->Height; j++)
			{
				fread(&tile, 1, 1, levelFile);
				level->Layout[i][j] = (0xF0 & tile) >> 4;
				level->Properties[i][j][0] = tile & 0x0F;
			}
		}

		fclose(levelFile);
	}

	loadTexture(&level->LevelTexture, textureFilename);

	// FIXME: Move metadata to .dat files.
	level->TextureClips = (RectangleF *)malloc(5 * sizeof(RectangleF));

	level->TextureClips[0].X = 0.0f;
	level->TextureClips[0].Y = 0.0f;

	level->TextureClips[1].X = BLOCK_SIZE;
	level->TextureClips[1].Y = 0.0f;

	level->TextureClips[2].X = 0.0f;
	level->TextureClips[2].Y = BLOCK_SIZE;

	level->TextureClips[3].X = BLOCK_SIZE;
	level->TextureClips[3].Y = BLOCK_SIZE;

	level->TextureClips[4].X = BLOCK_SIZE * 2;
	level->TextureClips[4].Y = 0.0f;

	for(int i = 0; i < 5; i++)
	{
		level->TextureClips[i].W = BLOCK_SIZE;
		level->TextureClips[i].H = BLOCK_SIZE;
	}

	level->Debug = false;
}

void saveLevel(Level *level, const char *filename)
{

}

void drawLevel(Level *level)
{
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			if((i >= camera_x / BLOCK_SIZE - 1) && (i < camera_x / BLOCK_SIZE + SCREEN_WIDTH / BLOCK_SIZE + 1) && (j >= camera_y / BLOCK_SIZE - 1) && (j < camera_y / BLOCK_SIZE + SCREEN_HEIGHT / BLOCK_SIZE + 1))
			{
				drawTexture(&level->LevelTexture, x, y, &level->TextureClips[level->Layout[i][j]], 0.0f);

				// TODO: Debug.
			}

			y += BLOCK_SIZE;
		}

		y = 0.0f;
		x += BLOCK_SIZE;
	}
}
