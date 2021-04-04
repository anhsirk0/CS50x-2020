#include "helpers.h"
#include <math.h>

typedef struct
{
    int x;
    int y;
} point;

point points[8];
point matrix[9];

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            BYTE red = image[h][w].rgbtRed;
            BYTE green = image[h][w].rgbtGreen;
            BYTE blue = image[h][w].rgbtBlue;
            BYTE new = round((red + green + blue) / 3.0);
            image[h][w].rgbtRed = new;
            image[h][w].rgbtGreen = new;
            image[h][w].rgbtBlue = new;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            new[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float c = 1.0;
            int red = new[h][w].rgbtRed;
            int green = new[h][w].rgbtGreen;
            int blue = new[h][w].rgbtBlue;

            points[0].x = h - 1;
            points[0].y = w - 1;

            points[1].x = h - 1;
            points[1].y = w ;

            points[2].x = h - 1;
            points[2].y = w + 1;

            points[3].x = h ;
            points[3].y = w - 1;

            points[4].x = h ;
            points[4].y = w + 1;

            points[5].x = h + 1;
            points[5].y = w - 1;

            points[6].x = h + 1;
            points[6].y = w ;

            points[7].x = h + 1;
            points[7].y = w + 1;

            for (int i = 0; i < 8; i++)
            {
                int a = points[i].x;
                int b = points[i].y;
                if (a >= 0 && a < height && b >= 0 && b < width)
                {
                    c++;
                    red += new[a][b].rgbtRed;
                    green += new[a][b].rgbtGreen;
                    blue += new[a][b].rgbtBlue;
                }
            }
            image[h][w].rgbtRed = round(red / c);
            image[h][w].rgbtGreen = round(green / c);
            image[h][w].rgbtBlue = round(blue / c);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    void update_matrix();
    RGBTRIPLE new[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            new[h][w] = image[h][w];
        }
    }
    // blur(height, width, new);
    int kernelx[9] = { - 1, 0, 1,
                       - 2, 0, 2,
                       - 1, 0, 1
                     };

    int kernely[9] = { - 1, - 2, - 1,
                       0, 0, 0,
                       1, 2, 1
                     };

    // int magx = 0;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            update_matrix(h, w);
            int redx = 0 ;
            int greenx = 0 ;
            int bluex = 0 ;

            int redy = 0 ;
            int greeny = 0 ;
            int bluey = 0 ;

            for (int i = 0; i < 9; i++)
            {
                int a = matrix[i].x;
                int b = matrix[i].y;
                if (a >= 0 && a < height && b >= 0 && b < width)
                {
                    redx += new[a][b].rgbtRed *kernelx[i] ;
                    greenx += new[a][b].rgbtGreen *kernelx[i] ;
                    bluex += new[a][b].rgbtBlue *kernelx[i] ;

                    redy += new[a][b].rgbtRed *kernely[i] ;
                    greeny += new[a][b].rgbtGreen *kernely[i] ;
                    bluey += new[a][b].rgbtBlue *kernely[i] ;
                }
            }

            int red = round(sqrt(redx * redx + redy * redy));
            int green = round(sqrt(greenx * greenx + greeny * greeny));
            int blue = round(sqrt(bluex * bluex + bluey * bluey));

            // int total = sqrt(avgx*avgx + avgy*avgy);

            image[h][w].rgbtRed = (red > 255) ? 255 : red;
            image[h][w].rgbtGreen = (green > 255) ? 255 : green;
            image[h][w].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }

    return;
}

void update_matrix(int h, int w)
{
    matrix[0].x = h - 1;
    matrix[0].y = w - 1;

    matrix[1].x = h - 1;
    matrix[1].y = w ;

    matrix[2].x = h - 1;
    matrix[2].y = w + 1;

    matrix[3].x = h ;
    matrix[3].y = w - 1;

    matrix[4].x = h ;
    matrix[4].y = w ;

    matrix[5].x = h ;
    matrix[5].y = w + 1;

    matrix[6].x = h + 1;
    matrix[6].y = w - 1;

    matrix[7].x = h + 1;
    matrix[7].y = w ;

    matrix[8].x = h + 1;
    matrix[8].y = w + 1;
}
