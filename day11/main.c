#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define BUFFER_COUNT 1024
#define INPUT_WIDTH 140
#define GALAXY_COUNT 452
#define EXPANSION 1000000

typedef struct Galaxy
{
    int64_t x;
    int64_t y;
} Galaxy;

int64_t absl(int64_t a)
{
    if (a < 0) return -a;
    return a;
}

void PartOne()
{
    char buffer[BUFFER_COUNT] = { 0 };

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    int x = 0;
    int y = 0;

    int hasGalaxyHorizontal = 0;

    // does the ith have a galaxy in it?
    int hasGalaxyVertical[INPUT_WIDTH] = { 0 };

    Galaxy galaxies[GALAXY_COUNT] = { 0 };
    int galaxiesIndex = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '#')
            {
                hasGalaxyHorizontal = 1;
                Galaxy g = { x, y };
                galaxies[galaxiesIndex] = g;
                galaxiesIndex++;

                hasGalaxyVertical[x] = 1;
            }

            if (c == '.' || c == '#')
            {
                x++;
                continue;
            }

            if (c == '\n')
            {
                if (!hasGalaxyHorizontal) y++;
                y++;
                x = 0;
                hasGalaxyHorizontal = 0;
                continue;
            }
        }
    }

    int horizontalOffset = 0;

    for (int i = 0; i < INPUT_WIDTH; i++)
    {
        int shouldIncrement = 0;
        if (!hasGalaxyVertical[i])
        {
            shouldIncrement = 1;
        }

        hasGalaxyVertical[i] = horizontalOffset;

        if (shouldIncrement)
        {
            horizontalOffset++;
        }
    }

    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        Galaxy g = galaxies[i];
        g.x += hasGalaxyVertical[g.x];
        galaxies[i] = g;
    }

    int sum = 0;

    for (int i = 0; i < GALAXY_COUNT - 1; i++)
    {
        for (int j = i + 1; j < GALAXY_COUNT; j++)
        {
            Galaxy gi = galaxies[i];
            Galaxy gj = galaxies[j];

            int diff = abs(gi.x - gj.x) + abs(gi.y - gj.y); // Manhattan distance
            sum += diff;
        }
    }

    printf("Sum: %d\n", sum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT] = { 0 };

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    int64_t x = 0;
    int64_t y = 0;

    int hasGalaxyHorizontal = 0;

    // does the ith have a galaxy in it?
    int64_t hasGalaxyVertical[INPUT_WIDTH] = { 0 };

    Galaxy galaxies[GALAXY_COUNT] = { 0 };
    int galaxiesIndex = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '#')
            {
                hasGalaxyHorizontal = 1;
                Galaxy g = { x, y };
                galaxies[galaxiesIndex] = g;
                galaxiesIndex++;

                hasGalaxyVertical[x] = 1;
            }

            if (c == '.' || c == '#')
            {
                x++;
                continue;
            }

            if (c == '\n')
            {
                y += hasGalaxyHorizontal ? 1 : EXPANSION;

                x = 0;
                hasGalaxyHorizontal = 0;
                continue;
            }
        }
    }

    int64_t horizontalOffset = 0;

    for (int i = 0; i < INPUT_WIDTH; i++)
    {
        int shouldIncrement = 0;
        if (!hasGalaxyVertical[i])
        {
            shouldIncrement = 1;
        }

        hasGalaxyVertical[i] = horizontalOffset * (EXPANSION - 1);

        if (shouldIncrement)
        {
            horizontalOffset++;
        }
    }

    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        Galaxy g = galaxies[i];
        g.x += hasGalaxyVertical[g.x];
        galaxies[i] = g;
    }

    int64_t sum = 0;

    for (int i = 0; i < GALAXY_COUNT - 1; i++)
    {
        for (int j = i + 1; j < GALAXY_COUNT; j++)
        {
            Galaxy gi = galaxies[i];
            Galaxy gj = galaxies[j];

            int64_t diff = absl(gi.x - gj.x) + absl(gi.y - gj.y);

            sum += diff;
        }
    }

    printf("Sum: %lld\n", sum);
}

int main()
{
    //PartOne();
    PartTwo();
    
    return 0;
}