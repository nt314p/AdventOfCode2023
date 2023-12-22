#include <stdio.h>
#include <stdint.h>

#define BUFFER_COUNT 1024
#define INPUT_COUNT 1000

typedef struct Input
{
    // For the bitfields, the LSB stores the rightmost spring
    uint32_t conditions; // Bits set if damaged
    uint32_t unknowns; // Bits set if unknown
    uint32_t length;
} Input;

int main()
{
    char buffer[BUFFER_COUNT] = { 0 };

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    Input inputs[INPUT_COUNT];
    int inputsIndex = 0;

    Input i = { 0 };

    int counts[INPUT_COUNT][6];
    int countsIndex = 0;

    int currentNumber = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == ' ')
            {
                inputs[inputsIndex] = i;

                printf("\nC: %u\n", i.conditions);
                printf("U: %u\n", i.unknowns);
                printf("L: %u\n", i.length);
                continue;
            }

            if (c >= '0' && c <= '9')
            {
                currentNumber *= 10;
                currentNumber += c - '0';
                continue;
            }

            if (c == ',' || c == '\n')
            {
                counts[inputsIndex][countsIndex] = currentNumber;
                countsIndex++;
                currentNumber = 0;
            }

            if (c == '\n')
            {
                inputsIndex++;
                countsIndex = 0;
                i.conditions = 0;
                i.unknowns = 0;
                i.length = 0;
            }

            i.conditions <<= 1;
            i.unknowns <<= 1;

            if (c == '#')
            {
                i.conditions |= 1;
            }

            if (c == '?')
            {
                i.unknowns |= 1;
            }

            i.length++;
        }
    }

    return 0;
}