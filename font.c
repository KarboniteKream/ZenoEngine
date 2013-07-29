#include "font.h"

void init_font(Font *font)
{
	init_texture(&font->texture);
	font->clips = NULL;
}

void load_font(Font *font, const char* filename)
{
	// NOTE: Instead of calling this function multiple times, I can load the list of fonts from a file.
	FILE *font_file = fopen(filename, "rb");

	if(font_file != NULL)
	{
		char texture_filename[256] = {'\0'};
		uint8_t filename_length, char_height, nnums;
		uint8_t flags[32];

		fread(&filename_length, sizeof(filename_length), 1, font_file);
		fread(texture_filename, sizeof(char), filename_length, font_file);
		load_texture(&font->texture, texture_filename);

		fread(&char_height, sizeof(char_height), 1, font_file);
		fread(&nnums, sizeof(nnums), 1, font_file);
		fread(flags, sizeof(uint8_t), (nnums + 7) / 8, font_file);

		if(font->clips != NULL)
		{
			free(font->clips);
		}

		font->clips = (Rectangle *)malloc(96 * sizeof(Rectangle));

		for(int i = 0, j = 0; i < nnums; i++)
		{
			int number = 0;
			fread(&number, flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, font_file);

			if(i % 2 == 0)
			{
				font->clips[i / 2].x = number;
				font->clips[i / 2].y = j;
			}
			else
			{
				font->clips[i / 2].w = number;
				font->clips[i / 2].h = char_height;

				// NOTE: Could this be simplified?
				if(i % 24 == 23)
				{
					j += char_height;
				}
			}
		}

		fclose(font_file);
	}
}

void draw_text(Font *font, GLfloat x, GLfloat y, const char* text)
{
	for(unsigned int i = 0; i < strlen(text); i++)
	{
		draw_texture(&font->texture, x, y, &font->clips[text[i] - 32], 0.0f);
		x += font->clips[text[i] - 32].w;
	}
}
