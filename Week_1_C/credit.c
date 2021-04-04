#include <stdio.h>
#include <cs50.h>

// main function
int main(void)
{
    int count();
    void credit();
    // printf("Number : ");
    long int card_number = get_long("Number : ");
    credit(card_number);
}

// count sum of digits
int count(int num)
{
    int sum = 0;
    while (num > 0)
    {
        sum = sum + num % 10;
        num = num / 10;
    }
    return sum;
}

// count number of digits
int length(long int num)
{
    int len = 0;
    while (num > 0)
    {
        len++;
        num = num / 10;
    }
    return len;
}

// return first two digits
int starts_with(long int num)
{
    while (num > 99)
    {
        num = num / 10;
    }
    return num;
}
// check if card number is valid or not
void credit(long int num)
{
    int l = length(num);
    int sw = starts_with(num);
    int i = 1;
    int sum = 0;
    int sum2 = 0;
    if (l == 14 || l < 13 || l > 16)
    {
        printf("INVALID\n");
        return;
    }
    while (num > 0)
    {
        if (i % 2 == 0)
        {
            sum = sum + count(2 * (num % 10));
        }
        else
        {
            sum2 = sum2 + num % 10;
        }
        num = num / 10;
        i++;
    }
    if ((sum + sum2) % 10 == 0)
    {
        if (l == 15 && (sw == 34 || sw == 37))
        {
            printf("AMEX\n");         
        }
        else if (sw / 10 == 4 && (l == 13 || l == 16))
        {
            printf("VISA\n");     
        }
        else if (sw > 50 && sw < 56 && l == 16)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
