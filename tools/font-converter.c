#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc >= 3)
	{
		FILE *input = fopen(argv[1], "r");
		FILE *output = fopen(argv[2], "wb");

		if(input != NULL && output != NULL)
		{
			char filename[256];
			uint8_t filename_length, nnums = 0, flags[32] = {0};
			uint16_t char_height, numbers[256];

			fscanf(input, "%s\n%hu", filename, &char_height);
			filename_length = strlen(filename);

			while(fscanf(input, "%hu", &numbers[nnums]) != EOF)
			{
				if(numbers[nnums] > 255)
				{
					flags[nnums / 8] |= 1 << (7 - nnums % 8);
				}

				nnums++;
			}

			fwrite(&filename_length, sizeof(filename_length), 1, output);
			fwrite(&filename, sizeof(char), filename_length, output);
			fwrite(&char_height, sizeof(uint8_t), 1, output);
			fwrite(&nnums, sizeof(nnums), 1, output);
			fwrite(&flags, sizeof(uint8_t), (nnums + 7) / 8, output);

			for(int i = 0; i < nnums; i++)
			{
				fwrite(&numbers[i], flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, output);
			}

			fclose(input);
			fclose(output);
		}
	}

	return 0;
}
