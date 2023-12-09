#include <stdio.h>
#include <stdint.h>

#define BUFFER_COUNT 1024
#define READINGS_COUNT 200
#define HISTORIES_COUNT 21

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file");
    }

    int readings[READINGS_COUNT][HISTORIES_COUNT] = { 0 };

    int readingsIndex = 0;
    int historiesIndex = 0;
    int currentValue = INT32_MAX;

    int isNegative = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '-') isNegative = 1;

            if (c >= '0' && c <= '9')
            {
                if (currentValue == INT32_MAX) currentValue = 0;
                currentValue *= 10;
                currentValue += c - '0';
            }

            if (c == ' ' || c == '\n')
            {
                if (isNegative) currentValue *= -1;
                readings[readingsIndex][historiesIndex] = currentValue;
                historiesIndex++;
                currentValue = INT32_MAX;
                isNegative = 0;
            }

            if (c == '\n')
            {
                readingsIndex++;
                historiesIndex = 0;
            }
        }
    }

    int sum = 0;

    for (int i = 0; i < READINGS_COUNT; i++)
    {
        int max = HISTORIES_COUNT - 1;
        while (1)
        {
            int allZero = 1;
            for (int j = 0; j < max; j++)
            {
                int diff = readings[i][j + 1] - readings[i][j];
                readings[i][j] = diff;
                allZero &= diff == 0;
            }

            if (allZero) break;

            max--;
        }

        for (int j = max + 1; j < HISTORIES_COUNT; j++)
        {
            readings[i][j] += readings[i][j - 1];
        }

        sum += readings[i][HISTORIES_COUNT - 1];
    }

    printf("Sum: %d\n", sum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file");
    }

    int readings[READINGS_COUNT][HISTORIES_COUNT] = { 0 };

    int readingsIndex = 0;
    int historiesIndex = 0;
    int currentValue = INT32_MAX;

    int isNegative = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '-') isNegative = 1;

            if (c >= '0' && c <= '9')
            {
                if (currentValue == INT32_MAX) currentValue = 0;
                currentValue *= 10;
                currentValue += c - '0';
            }

            if (c == ' ' || c == '\n')
            {
                if (isNegative) currentValue *= -1;
                readings[readingsIndex][historiesIndex] = currentValue;
                historiesIndex++;
                currentValue = INT32_MAX;
                isNegative = 0;
            }

            if (c == '\n')
            {
                readingsIndex++;
                historiesIndex = 0;
            }
        }
    }

    int sum = 0;

    for (int i = 0; i < READINGS_COUNT; i++)
    {
        int min = 1;

        while (1)
        {
            int allZero = 1;

            for (int j = HISTORIES_COUNT - 1; j >= min; j--)
            {
                int diff = readings[i][j] - readings[i][j - 1];
                readings[i][j] = diff;
                allZero &= diff == 0;
            }

            if (allZero) break;

            min++;
        }

        for (int j = min - 1; j >= 0; j--)
        {
            readings[i][j] -= readings[i][j + 1];
        }

        sum += readings[i][0];
    }

    printf("Sum: %d\n", sum);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}