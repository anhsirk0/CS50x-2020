#include <cs50.h>
#include <stdio.h>

int main(void)
{
    void mario();
    // int h;
    int x = 0;
    // infinite loop
    while (x == 0)
    {
        int h = get_int("Height: ");
        if (h > 0 && h < 9)
        {
            mario(h);
            // break after printing
            break;
        }    
    }
}

// mario function to draw
void mario(int height)
{
    for (int i = 1; i <= height; i++)
    {
        for (int j = i; j < height; j++)
        {
            printf(" ");            
        }
        for (int k = height - i; k < height; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
