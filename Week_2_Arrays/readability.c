#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
int main(void)
{
    int *count();
    int grade();
    // user input
    string text = get_string("Text : ");
    int *d = count(text);
    int g = grade(d[0], d[1], d[2]);
    if (g < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (g > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", g);
    }
}

// function to count words letters and sentences
int *count(char *text)
{
    int letters = 0;
    int words = 1;
    int sentences = 0;
    static int arr[3];
    long unsigned l = strlen(text);
    for (long unsigned i = 0; i < l; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters++;
        }
        if (text[i] == ' ')
        {
            words++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    arr[0] = letters;
    arr[1] = words;
    arr[2] = sentences;
    return arr;
}

// Coleman Liau index
int grade(int letters,  int words,  int sentences)
{
    float l = ((float)letters / (float)words) * 100;
    float s = ((float)sentences / (float)words) * 100;
    float index = 0.0588 * l - 0.296 * s - 15.8;
    return round(index);
}
