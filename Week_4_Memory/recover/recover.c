#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#define BLOCK_SIZE 512
#define NAME_SIZE 8

typedef uint8_t BYTE;

bool is_start();

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: . / recover image\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    bool first_image = false;
    int file_num = 0;
    FILE *outptr;

    while (fread(buffer, BLOCK_SIZE, 1, inptr))
    {
        if (is_start(buffer))
        {
            if (!first_image)
            {
                first_image = true;
            }
            else
            {
                fclose(outptr);
            }

            char filename[NAME_SIZE];
            sprintf(filename, "%03i.jpg", file_num++);

            outptr = fopen(filename, "a");
            if (outptr == NULL)
            {
                return 1;
            }

            fwrite(buffer, BLOCK_SIZE, 1, outptr);
        }
        else if (first_image)
        {
            fwrite(buffer, BLOCK_SIZE, 1, outptr);
        }
    }
    fclose(outptr);
    fclose(inptr);
}

bool is_start(BYTE buffer[])
{
    BYTE a = buffer[0];
    BYTE b = buffer[1];
    BYTE c = buffer[2];
    BYTE d = buffer[3];

    return (a == 255 && b == 216 && c == 255 && d > 223 && d < 240);
}

