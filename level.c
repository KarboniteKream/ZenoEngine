#include "level.h"

void init_level(Level *level)
{
	init_texture(&level->texture);
	level->texture_clips = NULL;

	level->width = 0;
	level->height = 0;

	level->layout = NULL;
	level->properties = NULL;

	level->debug = false;
}

void load_level(Level *level, const char *filename)
{
	char level_filename[256] = {'\0'};
	char texture_filename[256] = {'\0'};

	sprintf(level_filename, "levels/%s.map", filename);
	// FIXME: What about levels with same textures (regarding filename)?
	// NOTE: Should be resolved with moving metadata.
	sprintf(texture_filename, "images/%s-%d.png", filename, BLOCK_SIZE);

	// Move metadata to .dat files.
	level->width = 60;
	level->height = 40;

	if(level->layout != NULL)
	{
		for(int i = 0; i < level->width; i++)
		{
			for(int j = 0; j < level->height; j++)
			{
				free(level->properties[i][j]);
			}

			free(level->layout[i]);
			free(level->properties[i]);
		}

		free(level->texture_clips);
		free(level->layout);
		free(level->properties);
	}

	level->layout = (uint8_t **)malloc(level->width * sizeof(uint8_t *));
	level->properties = (uint8_t ***)malloc(level->width * sizeof(uint8_t **));

	for(int i = 0; i < level->width; i++)
	{
		level->layout[i] = (uint8_t *)calloc(level->height, sizeof(uint8_t));
		level->properties[i] = (uint8_t **)malloc(level->height * sizeof(uint8_t *));

		for(int j = 0; j < level->height; j++)
		{
			level->properties[i][j] = (uint8_t *)calloc(1, sizeof(uint8_t));
		}
	}

	FILE *level_file = fopen(level_filename, "rb");

	if(level_file != NULL)
	{
		int tile = 0;

		for(int i = 0; i < level->width; i++)
		{
			for(int j = 0; j < level->height; j++)
			{
				fread(&tile, 1, 1, level_file);
				level->layout[i][j] = (0xF0 & tile) >> 4;
				level->properties[i][j][0] = tile & 0x0F;
			}
		}

		fclose(level_file);
	}

	load_texture(&level->texture, texture_filename);

	// FIXME: Move metadata to .dat files.
	level->texture_clips = (Rectangle *)malloc(5 * sizeof(Rectangle));

	level->texture_clips[0].x = 0.0f;
	level->texture_clips[0].y = 0.0f;

	level->texture_clips[1].x = BLOCK_SIZE;
	level->texture_clips[1].y = 0.0f;

	level->texture_clips[2].x = 0.0f;
	level->texture_clips[2].y = BLOCK_SIZE;

	level->texture_clips[3].x = BLOCK_SIZE;
	level->texture_clips[3].y = BLOCK_SIZE;

	level->texture_clips[4].x = BLOCK_SIZE * 2;
	level->texture_clips[4].y = 0.0f;

	for(int i = 0; i < 5; i++)
	{
		level->texture_clips[i].w = BLOCK_SIZE;
		level->texture_clips[i].h = BLOCK_SIZE;
	}

	level->debug = false;
}

void save_level(Level *level, const char *filename)
{

}

void draw_level(Level *level)
{
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	for(int i = 0; i < level->width; i++)
	{
		for(int j = 0; j < level->height; j++)
		{
			if((i >= camera_x / BLOCK_SIZE - 1) && (i < camera_x / BLOCK_SIZE + SCREEN_WIDTH / BLOCK_SIZE + 1) && (j >= camera_y / BLOCK_SIZE - 1) && (j < camera_y / BLOCK_SIZE + SCREEN_HEIGHT / BLOCK_SIZE + 1))
			{
				draw_texture(&level->texture, x, y, &level->texture_clips[level->layout[i][j]], 0.0f);

				// TODO: debug.
				if(level->debug == true)
				{

				}
			}

			y += BLOCK_SIZE;
		}

		y = 0.0f;
		x += BLOCK_SIZE;
	}
}
