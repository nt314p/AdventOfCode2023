#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BUFFER_COUNT 1024

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int data[8];
    int dataIndex = 0;

    int currentNumber = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                currentNumber *= 10;
                currentNumber += c - '0';
                continue;
            }

            if (c == ' ' || c == '\n')
            {
                if (currentNumber == 0) continue;
                data[dataIndex] = currentNumber;
                dataIndex++;
                currentNumber = 0;
            }
        }
    }

    int product = 1;

    for (int i = 0; i < 4; i++)
    {
        int t = data[i];
        int d = data[i + 4];
        float u = t / 2.0f;
        float v = sqrtf(t * t - 4 * (d + 0.01f)) / 2;

        float top = u + v;
        float bottom = u - v;

        int topR = __min(floorf(top), t);
        int bottomR = ceilf(bottom);

        int count = topR - bottomR + 1;
        product *= count;
    }

    printf("Product: %d\n", product);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    long long data[2];
    int dataIndex = 0;

    long long currentNumber = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                currentNumber *= 10;
                currentNumber += c - '0';
                continue;
            }

            if (c == '\n')
            {
                if (currentNumber == 0) continue;
                data[dataIndex] = currentNumber;
                dataIndex++;
                currentNumber = 0;
            }
        }
    }

    long long t = data[0];
    long long d = data[1];
    float u = t / 2.0f;
    float v = sqrtf(t * t - 4 * (d + 0.01f)) / 2;

    float top = u + v;
    float bottom = u - v;

    long long topR = __min(floorf(top), t);
    long long bottomR = ceilf(bottom);

    long long count = topR - bottomR + 1;

    printf("Count: %ld\n", count);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}