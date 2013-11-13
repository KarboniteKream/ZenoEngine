#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv)
{
	if(argc >= 5)
	{
		FILE *input = fopen(argv[1], "rb");
		FILE *output = fopen(argv[2], "wb");

		if(input != NULL && output != NULL)
		{
			uint8_t width = atoi(argv[3]);
			uint8_t height = atoi(argv[4]);

			uint8_t matrix[width][height];

			for(int i = 0; i < width; i++)
			{
				for(int j = 0; j < height; j++)
				{
					fread(&matrix[i][j], 1, 1, input);
				}
			}

			for(int i = 0; i < height; i++)
			{
				for(int j = 0; j < width; j++)
				{
					fwrite(&matrix[j][i], 1, 1, output);
				}
			}
		}
	}

	return 0;
}
