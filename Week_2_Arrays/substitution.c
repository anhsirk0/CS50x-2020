#include <stdio.h>
#include <string.h>
#include <cs50.h>

int toupper(int);
int tolower(int);

int main(int argc, char *argv[])
{
    void encrypt();
    int check();
    if (argc == 1 || argc > 2)
    {
        printf("Usage: . / substitution key\n");
        return 1;
    }
    if (argc == 2)
    {
        if (strlen(argv[1]) != 26)
        {
            printf("INVALID KEY\n");
            return 1;
        }
        else
        {
            int r = check(argv[1]);
            if (r == 1)
            {
                printf("INVALID KEY\n");
                return 1;
            }
        }
    }
    char *key = argv[1];

    string msg = get_string("plaintext: ");
    encrypt(msg, key);
    printf("ciphertext: %s\n", msg);
    return 0;
}

// encrypt message function
void encrypt(char *text, char *key)
{
    long unsigned l = strlen(text);
    for (long unsigned i = 0; i < l; i++)
    {
        int c = text[i];
        if (c >= 'A' && c <= 'Z')
        {
            text[i] = key[c - 65];
        }
        if (c >= 'a' && c <= 'z')
        {
            text[i] = tolower(key[c - 97]);
        }
    }
}

// check the validity of key
int check(char *str)
{
    for (long unsigned i = 0; i < strlen(str); i++)
    {
        str[i] = toupper(str[i]);
        char c = str[i];
        if (c < 'A' || c > 'Z')
        {
            return 1;
        }
        for (long unsigned j = 0; j < i; j++)
        {
            if (str[j] == c)
            {
                return 1;
            }
        }
    }
    return 0;
}
