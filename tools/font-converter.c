#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc >= 3)
	{
		FILE *input = fopen(argv[1], "r");
		FILE *output = fopen(argv[2], "wb");

		if(input != NULL)
		{
			if(output != NULL)
			{
				char filename[256];
				uint8_t num = 0, flags[32] = {0};
				uint16_t charHeight, numbers[256];

				fscanf(input, "%255s\n%2hu", filename, &charHeight);
				uint8_t filenameLength = strlen(filename);

				while(fscanf(input, "%4hu", &numbers[num]) != EOF)
				{
					if(numbers[num] > 255)
					{
						flags[num / 8] |= 1 << (7 - num % 8);
					}

					num++;
				}

				fwrite(&filenameLength, sizeof(filenameLength), 1, output);
				fwrite(&filename, sizeof(char), filenameLength, output);
				fwrite(&charHeight, sizeof(uint8_t), 1, output);
				fwrite(&num, sizeof(num), 1, output);
				fwrite(&flags, sizeof(uint8_t), (num + 7) / 8, output);

				for(int i = 0; i < num; i++)
				{
					fwrite(&numbers[i], flags[i / 8] & (1 << (7 - i % 8)) ? sizeof(uint16_t) : sizeof(uint8_t), 1, output);
				}

				fclose(output);
			}

			fclose(input);
		}
	}

	return 0;
}
