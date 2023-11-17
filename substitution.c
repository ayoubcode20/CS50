#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int validate_key(char *key);

/**
 * main - a program that encrypts messages using a substitution cipher.
 * @argc: number of arguments
 * @argv: the arguments
 *
 * Return: zero in success, otherwise 1.
 */

int main(int argc, char *argv[])
{
    int i, ch;

    // validate key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return (1);
    }

    i = validate_key(argv[1]);
    if (i)
        return (1);

    // get plaintext
    char *text = malloc(sizeof(char) * 1024);

    if (!text)
    {
	    printf("failed to allocate memory.\n");
	    return (1);
    }
    printf("plaintext:  ");
    fgets(text, sizeof(char) * 1024, stdin);

    // Encipher
    for (i = 0; text[i]; i++)
    {
        ch = text[i];
        if (isupper(ch))
        {
            text[i] = argv[1][ch - 65];
            text[i] = toupper(text[i]);
        }
        else if (islower(ch))
        {
            text[i] = argv[1][ch - 97];
            text[i] = tolower(text[i]);
        }
    }

    // print ciphertext
    printf("ciphertext: %s", text);

    return (0);
}

/**
 * Validates the substitution cipher key to ensure it meets the required criteria:
 * - Exactly 26 characters
 * - Contains only alphabetic characters
 * - Does not have repeated characters
 *
 * @param key The substitution cipher key to be validated.
 * @return 0 if the key is valid, 1 otherwise.
 */

int validate_key(char *key)
{
    int i, j;

    for (i = 0; key[i]; i++)
    {
        if (i == 26)
        {
            printf("key must only contain 26 characters.\n");
            return (1);
        }
        if (!isalpha(key[i]))
        {
            printf("key must only contain alphabetic characters.\n");
            return (1);
        }
    }
    if (i != 26)
    {
        printf("key must contain 26 characters.\n");
        return (1);
    }

    for (i = 0; i < 26; i++)
    {
        for (j = i + 1; j < 26; j++)
        {
            if (key[i] == key[j])
            {
                printf("key must not contain repeated characters.\n");
                return (1);
            }
        }
    }

    return (0);
}
