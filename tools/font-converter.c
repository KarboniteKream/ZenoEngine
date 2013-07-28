#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv)
{
	if(argc >= 3)
	{
		FILE *input = fopen(argv[1], "r");

		if(input != NULL)
		{
			char image_filename[256];
			uint8_t char_height, nnums = 0, flags[32] = {0};
			uint16_t numbers[256];

			fscanf(input, "%s\n%hhd", image_filename, &char_height);

			while(fscanf(input, "%hd", &numbers[nnums]) != EOF)
			{
				if(numbers[nnums] > 255)
				{
					flags[nnums / 8] |= 1 << (7 - nnums % 8);
				}

				nnums++;
			}

			fclose(input);

			FILE *output = fopen(argv[2], "wb");

			if(output != NULL)
			{
				fprintf(output, "%s\n", image_filename);
				fwrite(&char_height, sizeof(char_height), 1, output);
				fwrite(&nnums, sizeof(nnums), 1, output);
				fwrite(&flags, sizeof(uint8_t), (nnums + 7) / 8, output);

				for(int i = 0; i < nnums; i++)
				{
					fwrite(&numbers[i], flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, output);
				}

				fclose(output);
			}
		}
	}

	return 0;
}
