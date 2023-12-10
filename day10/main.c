#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_COUNT 1024
#define PIPES_LENGTH 140
#define MAIN_PIPES_COUNT 13364 // more hardcoding

void PrintGrid(char* pipes, int* mainPipeIndices);

char Direction(char pipe, char inDirection)
{
    if (pipe == '|')
    {
        if (inDirection == 'N') return 'N';
        if (inDirection == 'S') return 'S';
    }
    else if (pipe == '-')
    {
        if (inDirection == 'E') return 'E';
        if (inDirection == 'W') return 'W';
    }
    else if (pipe == 'F')
    {
        if (inDirection == 'N') return 'E';
        if (inDirection == 'W') return 'S';
    }
    else if (pipe == '7')
    {
        if (inDirection == 'E') return 'S';
        if (inDirection == 'N') return 'W';
    }
    else if (pipe == 'J')
    {
        if (inDirection == 'S') return 'W';
        if (inDirection == 'E') return 'N';
    }
    else if (pipe == 'L')
    {
        if (inDirection == 'S') return 'E';
        if (inDirection == 'W') return 'N';
    }

    return '0';
}

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    char pipes[PIPES_LENGTH * PIPES_LENGTH];
    int pipesIndex = 0;
    int startingIndex = -1;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '\n') continue;
            if (c == 'S') startingIndex = pipesIndex;
            pipes[pipesIndex] = c;
            pipesIndex++;
        }
    }

    int count = 0;
    pipesIndex = startingIndex;
    char dir = 'E'; // N S E W, hardcoded to start east

    while (1)
    {
        if (dir == 'E') pipesIndex++;
        if (dir == 'W') pipesIndex--;
        if (dir == 'N') pipesIndex -= PIPES_LENGTH;
        if (dir == 'S') pipesIndex += PIPES_LENGTH;

        char pipe = pipes[pipesIndex];
        if (pipe == 'S') break;

        dir = Direction(pipe, dir);
        count++;
    }

    printf("Count: %d\n", count);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    char pipes[PIPES_LENGTH * PIPES_LENGTH];
    int pipesIndex = 0;
    int startingIndex = -1;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '\n') continue;
            if (c == 'S') startingIndex = pipesIndex;
            pipes[pipesIndex] = c;
            pipesIndex++;
        }
    }

    int count = 0;
    pipesIndex = startingIndex; // 10553
    char dir = 'E'; // N S E W, hardcoded to start east

    int mainPipeIndices[MAIN_PIPES_COUNT];

    while (1)
    {
        if (dir == 'E') pipesIndex++;
        if (dir == 'W') pipesIndex--;
        if (dir == 'N') pipesIndex -= PIPES_LENGTH;
        if (dir == 'S') pipesIndex += PIPES_LENGTH;

        char pipe = pipes[pipesIndex];
        mainPipeIndices[count] = pipesIndex;

        if (pipe == 'S') break;

        dir = Direction(pipe, dir);
        count++;
    }

    pipes[startingIndex] = '-'; // More hardcode replace

    // Sort all pipes that belong to the main loop
    for (int i = 0; i < MAIN_PIPES_COUNT - 1; i++)
    {
        int lowestValue = mainPipeIndices[i];
        int lowestIndex = i;

        for (int j = i + 1; j < MAIN_PIPES_COUNT; j++)
        {
            int value = mainPipeIndices[j];
            if (value < lowestValue)
            {
                lowestValue = value;
                lowestIndex = j;
            }
        }

        if (i == lowestIndex) continue;
        mainPipeIndices[lowestIndex] = mainPipeIndices[i];
        mainPipeIndices[i] = lowestValue;
    }

    int area = 0;
    pipesIndex = 0;
    int mainPipesIndexIndex = 0;
    int inside = 0;

    // Iterate through each row...
    // We start on the Outside, and if we encounter a certain pipe,
    // we know we're on the Inside of the loop
    // OOOOO|III|OOOF----7OOO|III...
    // The | will always switch outside to inside
    // The 7 and J will only switch if the starting pipe was opposite
    // So F---J switches and L---7 switches, but F---7 and L---J does not

    for (int row = 0; row < PIPES_LENGTH; row++)
    {
        int previousPipeIndex = 0;
        inside = 0;
        char prevPipeAngle = '0';

        while (1)
        {
            int mainPipesIndex = mainPipeIndices[mainPipesIndexIndex];
            if (mainPipesIndex / PIPES_LENGTH < row) // Main pipe index too small
            {
                mainPipesIndexIndex++;
                continue;
            }

            // Row too small
            if (mainPipesIndex / PIPES_LENGTH >= row + 1) break;

            char pipe = pipes[mainPipesIndex];
            int diff = mainPipesIndex - previousPipeIndex - 1;

            if (pipe == 'F' || pipe == 'L') // Found starting angle pipe
            {
                prevPipeAngle = pipe; // Remember pipe

                // This could be a transition to outside, add
                if (inside) area += diff;
            }

            // |,J,7 are the only pipes that could possibly cause a transition
            if (pipe != '|' && pipe != 'J' && pipe != '7')
            {
                mainPipesIndexIndex++;
                previousPipeIndex = mainPipesIndex;
                continue;
            }

            if (inside) area += diff; // Could be a transition to outside


            // Pipes that go F---7 or L---J do not cause a net transition
            if ((pipe == '7' && prevPipeAngle == 'F') ||
                (pipe == 'J' && prevPipeAngle == 'L')) {
                inside ^= 1; // flip inside to negate the next flip
            }

            inside ^= 1;

            previousPipeIndex = mainPipesIndex;
            mainPipesIndexIndex++;
        }
    }

    printf("Area: %d\n", area);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}

void PrintGrid(char* pipes, int* mainPipeIndices)
{
    for (int i = 0; i < PIPES_LENGTH * PIPES_LENGTH; i++)
    {
        int found = 0;
        for (int j = 0; j < MAIN_PIPES_COUNT; j++)
        {
            found |= mainPipeIndices[j] == i;
        }
        if (!found) pipes[i] = ' ';
    }

    for (int row = 0; row < PIPES_LENGTH; row++)
    {
        for (int col = 0; col < PIPES_LENGTH; col++)
        {
            char p = pipes[row * PIPES_LENGTH + col];

            if (p == 'F') printf("╔");
            if (p == '7') printf("╗");
            if (p == 'L') printf("╚");
            if (p == 'J') printf("╝");
            if (p == '|') printf("║");
            if (p == '-') printf("═");
            if (p == ' ') printf(" ");
        }
        printf("\n");
    }
}