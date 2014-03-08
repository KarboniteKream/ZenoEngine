#include "level.h"

void initLevel(Level *level)
{
	level->Width = 0;
	level->Height = 0;

	level->Data = NULL;

	initTexture(&level->LevelTexture);
	level->TexClips = NULL;
}

// TODO: Free level.
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

	if(level->Data != NULL)
	{
		for(int i = 0; i < level->Width; i++)
		{
			for(int j = 0; j < level->Height; j++)
			{
				free(level->Data[i][j]);
			}

			free(level->Data[i]);
		}

		free(level->Data);
		free(level->TexClips);
	}

	level->Data = (uint8_t ***)malloc(level->Width * sizeof(uint8_t **));

	for(int i = 0; i < level->Width; i++)
	{
		level->Data[i] = (uint8_t **)malloc(level->Height * sizeof(uint8_t *));

		for(int j = 0; j < level->Height; j++)
		{
			level->Data[i][j] = (uint8_t *)calloc(2, sizeof(uint8_t));
		}
	}

	FILE *levelFile = fopen(levelFilename, "rb");

	if(levelFile != NULL)
	{
		// NOTE: Should this be uint8_t?
		int tile = 0;

		for(int i = 0; i < level->Width; i++)
		{
			for(int j = 0; j < level->Height; j++)
			{
				fread(&tile, 1, 1, levelFile);
				level->Data[i][j][LAYOUT] = (tile & 0xF0) >> 4;
				level->Data[i][j][PROPERTIES] = tile & 0x0F;
			}
		}

		fclose(levelFile);
	}

	loadTexture(&level->LevelTexture, textureFilename);

	// FIXME: Move metadata to .dat files.
	level->TexClips = (RectangleF *)malloc(7 * sizeof(RectangleF));

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

	level->TexClips[5].X = BLOCK_SIZE * 2;
	level->TexClips[5].Y = BLOCK_SIZE;

	level->TexClips[6].X = 0.0f;
	level->TexClips[6].Y = BLOCK_SIZE * 2;

	for(int i = 0; i < 7; i++)
	{
		level->TexClips[i].W = BLOCK_SIZE;
		level->TexClips[i].H = BLOCK_SIZE;
	}

	generateLevelVBO(level);
}

void saveLevel(Level *level, const char *filename)
{
	char levelFilename[256] = {'\0'};

	// FIXME: What about levels with same textures (regarding filename)?
	// NOTE: Should be resolved with moving metadata.
	sprintf(levelFilename, "levels/%s.map", filename);

	FILE *levelFile = fopen(levelFilename, "wb");

	if(levelFile != NULL)
	{
		// NOTE: Should this be uint8_t?
		int tile = 0;

		for(int i = 0; i < level->Width; i++)
		{
			for(int j = 0; j < level->Height; j++)
			{
				tile = (level->Data[i][j][LAYOUT] << 4) + (level->Data[i][j][PROPERTIES] & 0x0F);
				fwrite(&tile, 1, 1, levelFile);
			}
		}

		fclose(levelFile);
	}
}

void generateLevelVBO(Level *level)
{
	VertexData vertexData[4 * level->Width * level->Height];

	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			int index = (level->Height * i + j) * 4;

			// TODO: Move texture S and T into separate variable, so there's no need for calculation.
			vertexData[index].X = i * BLOCK_SIZE;
			vertexData[index].Y = j * BLOCK_SIZE;
			vertexData[index].S = level->TexClips[level->Data[i][j][LAYOUT]].X / level->LevelTexture.TexWidth;
			vertexData[index].T = level->TexClips[level->Data[i][j][LAYOUT]].Y / level->LevelTexture.TexHeight;

			vertexData[index + 1].X = i * BLOCK_SIZE + level->TexClips[level->Data[i][j][LAYOUT]].W;
			vertexData[index + 1].Y = j * BLOCK_SIZE;
			vertexData[index + 1].S = (level->TexClips[level->Data[i][j][LAYOUT]].X + level->TexClips[level->Data[i][j][LAYOUT]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 1].T = level->TexClips[level->Data[i][j][LAYOUT]].Y / level->LevelTexture.TexHeight;

			vertexData[index + 2].X = i * BLOCK_SIZE + level->TexClips[level->Data[i][j][LAYOUT]].W;
			vertexData[index + 2].Y = j * BLOCK_SIZE + level->TexClips[level->Data[i][j][LAYOUT]].H;
			vertexData[index + 2].S = (level->TexClips[level->Data[i][j][LAYOUT]].X + level->TexClips[level->Data[i][j][LAYOUT]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 2].T = (level->TexClips[level->Data[i][j][LAYOUT]].Y + level->TexClips[level->Data[i][j][LAYOUT]].H) / level->LevelTexture.TexHeight;

			vertexData[index + 3].X = i * BLOCK_SIZE;
			vertexData[index + 3].Y = j * BLOCK_SIZE + level->TexClips[level->Data[i][j][LAYOUT]].H;
			vertexData[index + 3].S = level->TexClips[level->Data[i][j][LAYOUT]].X / level->LevelTexture.TexWidth;
			vertexData[index + 3].T = (level->TexClips[level->Data[i][j][LAYOUT]].Y + level->TexClips[level->Data[i][j][LAYOUT]].H) / level->LevelTexture.TexHeight;
		}
	}

	initVBO(&level->LevelTexture, vertexData, level->Width * level->Height);
}

void drawLevel(Level *level)
{
	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			if((i >= cameraX / BLOCK_SIZE - 1) && (i < cameraX / BLOCK_SIZE + SCREEN_WIDTH / BLOCK_SIZE + 1) && (j >= cameraY / BLOCK_SIZE - 1) && (j < cameraY / BLOCK_SIZE + SCREEN_HEIGHT / BLOCK_SIZE + 1))
			{
				drawTexture(&level->LevelTexture, i * BLOCK_SIZE, j * BLOCK_SIZE, &level->TexClips[level->Data[i][j][0]], 0.0f, 1.0f, false);

				// TODO: Debug.
				if(DEBUG == true)
				{

				}
			}
		}
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
	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
	glVertexPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
	glDrawArrays(GL_QUADS, 0, 4 * level->Width * level->Height);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void setLevelLayout(Level *level, int x, int y, int block)
{
	// NOTE: Do I even need Layout?
	level->Data[x][y][0] = block;

	switch(block)
	{
		case 0:
			level->Data[x][y][PROPERTIES] = 0;
		break;

		case 1:
			level->Data[x][y][PROPERTIES] = 0;
		break;

		case 2:
			level->Data[x][y][PROPERTIES] = 0;
		break;

		case 3:
			level->Data[x][y][PROPERTIES] = 0;
		break;

		case 4:
			level->Data[x][y][PROPERTIES] = COLLIDABLE;
		break;

		case 5:
			level->Data[x][y][PROPERTIES] = SLOPE_LEFT;
		break;

		case 6:
			level->Data[x][y][PROPERTIES] = SLOPE_RIGHT;
		break;
	}

	// FIXME: Only change S and T.
	VertexData vertexData[4];

	vertexData[0].X = x * BLOCK_SIZE;
	vertexData[0].Y = y * BLOCK_SIZE;
	vertexData[0].S = level->TexClips[block].X / level->LevelTexture.TexWidth;
	vertexData[0].T = level->TexClips[block].Y / level->LevelTexture.TexHeight;

	vertexData[1].X = x * BLOCK_SIZE + level->TexClips[block].W;
	vertexData[1].Y = y * BLOCK_SIZE;
	vertexData[1].S = (level->TexClips[block].X + level->TexClips[block].W) / level->LevelTexture.TexWidth;
	vertexData[1].T = level->TexClips[block].Y / level->LevelTexture.TexHeight;

	vertexData[2].X = x * BLOCK_SIZE + level->TexClips[block].W;
	vertexData[2].Y = y * BLOCK_SIZE + level->TexClips[block].H;
	vertexData[2].S = (level->TexClips[block].X + level->TexClips[block].W) / level->LevelTexture.TexWidth;
	vertexData[2].T = (level->TexClips[block].Y + level->TexClips[block].H) / level->LevelTexture.TexHeight;

	vertexData[3].X = x * BLOCK_SIZE;
	vertexData[3].Y = y * BLOCK_SIZE + level->TexClips[block].H;
	vertexData[3].S = level->TexClips[block].X / level->LevelTexture.TexWidth;
	vertexData[3].T = (level->TexClips[block].Y + level->TexClips[block].H) / level->LevelTexture.TexHeight;

	glBindBuffer(GL_ARRAY_BUFFER, level->LevelTexture.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, (level->Height * x + y) * 4 * sizeof(VertexData), 4 * sizeof(VertexData), vertexData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
