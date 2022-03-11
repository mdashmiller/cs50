#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Prototypes
int find_jpeg(FILE *file);

typedef uint8_t BYTE;
const int BLOCK = 512;

int main(int argc, char *argv[])
{
    // Check for proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open card image file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Storage for each block as it is read from file
    BYTE buffer[BLOCK];
    // Pointer for the files we will write to
    FILE *img;
    // Container for string for file names
    char filename[9];
    // Track number of jpegs found
    int counter = 0;

    while (fread(&buffer, sizeof(BYTE), BLOCK, file) == BLOCK)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter == 0)
            {
                // Create the first jpeg file to write to
                sprintf(filename, "%03i.jpeg", counter);
                counter ++;

                img = fopen(filename, "w");

                fwrite(&buffer, sizeof(BYTE), BLOCK, img);
            }
            else
            {
                // Close the current jpeg file and create another
                fclose(img);

                sprintf(filename, "%03i.jpeg", counter);
                counter ++;

                img = fopen(filename, "w");

                fwrite(&buffer, sizeof(BYTE), BLOCK, img);
            }
        }
        else
        {
            if (counter > 0)
            {
                // Already found a jpeg so keep writing to it
                fclose(img);

                img = fopen(filename, "a");

                fwrite(&buffer, sizeof(BYTE), BLOCK, img);
            }
        }
    }
    fclose(img);
    fclose(file);

    return 0;
}
