#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO
    int i, j, ch, k;
    int arr[BITS_IN_BYTE];
    char *str = malloc(sizeof(char) * 1024);

    if (!str)
            return (1);
    printf("Message: ");
    fgets(str, sizeof(char) * 1024, stdin);

    for (i = 0; str[i]; i++)
    {
        ch = str[i];
        for (j = 0; j < BITS_IN_BYTE; j++)
        {
            arr[j] = ch % 2;
            ch /= 2;
        }
        for (k = BITS_IN_BYTE - 1; k >= 0; k--)
            print_bulb(arr[k]);
        printf("\n");
    }

    free(str);
    return (0);

}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
