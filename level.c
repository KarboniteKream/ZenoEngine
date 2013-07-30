#include "level.h"

void initLevel(Level *level)
{
	initTexture(&level->LevelTexture);
	level->TexClips = NULL;

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

		free(level->TexClips);
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
	level->TexClips = (Rectangle *)malloc(5 * sizeof(Rectangle));

	level->TexClips[0].X = 0.0f;
	level->TexClips[0].Y = 0.0f;

	level->TexClips[1].X = BLOCK_SIZE;
	level->TexClips[1].Y = 0.0f;

	level->TexClips[2].X = 0.0f;
	level->TexClips[2].Y = BLOCK_SIZE;

	level->TexClips[3].X = BLOCK_SIZE;
	level->TexClips[3].Y = BLOCK_SIZE;

	level->TexClips[4].X = BLOCK_SIZE * 2;
	level->TexClips[4].Y = 0.0f;

	for(int i = 0; i < 5; i++)
	{
		level->TexClips[i].W = BLOCK_SIZE;
		level->TexClips[i].H = BLOCK_SIZE;
	}

	level->Debug = false;

	generateLevelVBO(level);
}

void saveLevel(Level *level, const char *filename)
{

}

void generateLevelVBO(Level *level)
{
	VertexData vertexData[4 * level->Width * level->Height];
	GLuint indexData[4 * level->Width * level->Height];

	for(int i = 0; i < 4 * level->Width * level->Height; i++)
	{
		indexData[i] = i;
	}

	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			int index = (level->Height * i + j) * 4;

			vertexData[index].X = x;
			vertexData[index].Y = y;
			vertexData[index].TX = level->TexClips[level->Layout[i][j]].X / level->LevelTexture.TexWidth;
			vertexData[index].TY = level->TexClips[level->Layout[i][j]].Y / level->LevelTexture.TexHeight;

			vertexData[index + 1].X = x + level->TexClips[level->Layout[i][j]].W;
			vertexData[index + 1].Y = y;
			vertexData[index + 1].TX = (level->TexClips[level->Layout[i][j]].X + level->TexClips[level->Layout[i][j]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 1].TY = level->TexClips[level->Layout[i][j]].Y / level->LevelTexture.TexHeight;

			vertexData[index + 2].X = x + level->TexClips[level->Layout[i][j]].W;
			vertexData[index + 2].Y = y + level->TexClips[level->Layout[i][j]].H;
			vertexData[index + 2].TX = (level->TexClips[level->Layout[i][j]].X + level->TexClips[level->Layout[i][j]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 2].TY = (level->TexClips[level->Layout[i][j]].Y + level->TexClips[level->Layout[i][j]].H) / level->LevelTexture.TexHeight;

			vertexData[index + 3].X = x;
			vertexData[index + 3].Y = y + level->TexClips[level->Layout[i][j]].H;
			vertexData[index + 3].TX = level->TexClips[level->Layout[i][j]].X / level->LevelTexture.TexWidth;
			vertexData[index + 3].TY = (level->TexClips[level->Layout[i][j]].Y + level->TexClips[level->Layout[i][j]].H) / level->LevelTexture.TexHeight;

			y += BLOCK_SIZE;
		}

		y = 0.0f;
		x += BLOCK_SIZE;
	}

	initStaticVBO(&level->LevelTexture, vertexData, indexData, level->Width * level->Height);
}

void drawLevel(Level *level)
{
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			if((i >= cameraX / BLOCK_SIZE - 1) && (i < cameraX / BLOCK_SIZE + SCREEN_WIDTH / BLOCK_SIZE + 1) && (j >= cameraY / BLOCK_SIZE - 1) && (j < cameraY / BLOCK_SIZE + SCREEN_HEIGHT / BLOCK_SIZE + 1))
			{
				drawTexture(&level->LevelTexture, x, y, &level->TexClips[level->Layout[i][j]], 0.0f);

				// TODO: Debug.
				if(level->Debug == true)
				{

				}
			}

			y += BLOCK_SIZE;
		}

		y = 0.0f;
		x += BLOCK_SIZE;
	}
}

void drawLevelVBO(Level *level)
{
	// TODO: Move to texture.c.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);

	if(BOUND_TEXTURE != level->LevelTexture.ID)
	{
		glBindTexture(GL_TEXTURE_2D, level->LevelTexture.ID);
		BOUND_TEXTURE = level->LevelTexture.ID;
	}

	glBindBuffer(GL_ARRAY_BUFFER, level->LevelTexture.VBO);

	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, TX));
	glVertexPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, level->LevelTexture.IBO);
	glDrawElements(GL_QUADS, 4 * level->Width * level->Height, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
