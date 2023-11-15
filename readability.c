#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

/**
 * main - a program that calculates the approximate
 * grade level needed to comprehend some text
 *
 * Return: Always zero (success)
 */

int main(void)
{
    int i;
    double L = 0, W = 1, S = 0;
    double grade;
    string text = get_string("Text: ");

    // count number of letters, words and sentences
    for (i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
            L++;
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
            S++;
        if (text[i] == ' ' && text[i + 1] != ' ')
            W++;
    }

    // count the average number of letters and sentences per 100 words
    L = L / W * 100;
    S = S / W * 100;

    // count the grade using The Coleman-Liau index formula
    grade = 0.0588 * L - 0.296 * S - 15.8;

    // round the number to the nearest whole numebr
    grade = grade + 0.5;

    // output the result to the user
    if (grade >= 16)
        printf("Grade 16+\n");
    else if (grade < 1)
        printf("Before Grade 1\n");
    else
        printf("Grade %i\n", (int) grade);

    return (0);
}
