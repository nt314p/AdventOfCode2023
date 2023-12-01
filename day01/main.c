#include <stdio.h>
#include "string.h"

#define BUFFER_COUNT 1024

void PartOne() {
    char buffer[BUFFER_COUNT];
    unsigned int sum = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int lastNumber = -1;

    // chunked read
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0;; index++)
        {
            char c = buffer[index];
            if (c == '\0') break; // end of buffer, break and read more

            if (c == '\n') // end of line, last number
            {
                sum += lastNumber;
                lastNumber = -1; // indicate new line
            }

            if (c < '0' || c > '9') continue; // input is not a digit

            int value = (c - '0'); // convert to int

            // lastNumber == -1 indicates newline, first number
            if (lastNumber == -1) sum += 10 * value;

            lastNumber = value;
        }
    }

    sum += lastNumber;

    printf("%d", sum);
}

void PartTwo()
{
    char* table[] = {
        "*",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    int lengths[] = {
        0,
        3,
        3,
        5,
        4,
        4,
        3,
        5,
        5,
        4,
        0
    };

    char buffer[BUFFER_COUNT];
    unsigned int sum = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int lastNumber = -1;

    // chunked read
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        int index = 0;
        while (1)
        {
            char c = buffer[index];
            if (c == '\0') break; // end of buffer, break and read more

            if (c == '\n') // end of line, last number
            {
                sum += lastNumber;
                lastNumber = -1; // indicate new line
            }

            int value;

            if (c >= '0' && c <= '9') {
                value = (c - '0'); // convert to int
                index++;
            }
            else
            {
                // parse string numbers
                for (value = 1; value < 10; value++)
                {
                    if (strncmp(buffer + index, table[value], lengths[value]) == 0) break;
                }
                //printf("Value: %d\n", value);
                if (value == 10) // did not match
                {
                    index++;
                    continue;
                }
                else
                {
                    index += lengths[value] - 1; // because "twone"
                }
            }

            // lastNumber == -1 indicates newline, first number
            if (lastNumber == -1)
            {
                sum += 10 * value;
            }

            lastNumber = value;
        }
    }

    sum += lastNumber;

    printf("%d", sum);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}