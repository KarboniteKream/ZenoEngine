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
				level->Layout[i][j] = (tile & 0xF0) >> 4;
				// NOTE: This assigns all the properties to the 'collidable' variable.
				level->Properties[i][j][0] = tile & 0x0F;
			}
		}

		fclose(levelFile);
	}

	loadTexture(&level->LevelTexture, textureFilename);

	// FIXME: Move metadata to .dat files.
	level->TexClips = (RectangleF *)malloc(5 * sizeof(RectangleF));

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

	for(int i = 0; i < level->Width; i++)
	{
		for(int j = 0; j < level->Height; j++)
		{
			int index = (level->Height * i + j) * 4;

			// TODO: Move texture S and T into separate variable, so there's no need for calculation.
			vertexData[index].X = i * BLOCK_SIZE;
			vertexData[index].Y = j * BLOCK_SIZE;
			vertexData[index].S = level->TexClips[level->Layout[i][j]].X / level->LevelTexture.TexWidth;
			vertexData[index].T = level->TexClips[level->Layout[i][j]].Y / level->LevelTexture.TexHeight;

			// NOTE: What about BLOCK_SIZE instead?
			vertexData[index + 1].X = i * BLOCK_SIZE + level->TexClips[level->Layout[i][j]].W;
			vertexData[index + 1].Y = j * BLOCK_SIZE;
			vertexData[index + 1].S = (level->TexClips[level->Layout[i][j]].X + level->TexClips[level->Layout[i][j]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 1].T = level->TexClips[level->Layout[i][j]].Y / level->LevelTexture.TexHeight;

			vertexData[index + 2].X = i * BLOCK_SIZE + level->TexClips[level->Layout[i][j]].W;
			vertexData[index + 2].Y = j * BLOCK_SIZE + level->TexClips[level->Layout[i][j]].H;
			vertexData[index + 2].S = (level->TexClips[level->Layout[i][j]].X + level->TexClips[level->Layout[i][j]].W) / level->LevelTexture.TexWidth;
			vertexData[index + 2].T = (level->TexClips[level->Layout[i][j]].Y + level->TexClips[level->Layout[i][j]].H) / level->LevelTexture.TexHeight;

			vertexData[index + 3].X = i * BLOCK_SIZE;
			vertexData[index + 3].Y = j * BLOCK_SIZE + level->TexClips[level->Layout[i][j]].H;
			vertexData[index + 3].S = level->TexClips[level->Layout[i][j]].X / level->LevelTexture.TexWidth;
			vertexData[index + 3].T = (level->TexClips[level->Layout[i][j]].Y + level->TexClips[level->Layout[i][j]].H) / level->LevelTexture.TexHeight;
		}
	}

	initStaticVBO(&level->LevelTexture, vertexData, level->Width * level->Height);
}

// TODO: i * BLOCK_SIZE
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
				drawTexture(&level->LevelTexture, x, y, &level->TexClips[level->Layout[i][j]], 0.0f, 1.0f);

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

	glBindBufferARB(GL_ARRAY_BUFFER, level->LevelTexture.VBO);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, S));
	glVertexPointer(2, GL_FLOAT, sizeof(VertexData), (GLvoid *)offsetof(VertexData, X));
	glDrawArrays(GL_QUADS, 0, 4 * level->Width * level->Height);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void setLevelLayout(Level *level, int x, int y, int block)
{
	// NOTE: Do I even need Layout?
	level->Layout[x][y] = block;

	switch(block)
	{
		case 0:
			level->Properties[x][y][0] = 0;
		break;

		case 1:
			level->Properties[x][y][0] = 0;
		break;

		case 2:
			level->Properties[x][y][0] = 0;
		break;

		case 3:
			level->Properties[x][y][0] = 2;
		break;

		case 4:
			level->Properties[x][y][0] = 1;
		break;
	}

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

	glBindBufferARB(GL_ARRAY_BUFFER, level->LevelTexture.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, (level->Height * x + y) * 4 * sizeof(VertexData), 4 * sizeof(VertexData), vertexData);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
}
