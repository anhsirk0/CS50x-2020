#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // ask name
    string name = get_string("What is your name?\n");

    // print hello world
    printf("%s\n", name);
}
