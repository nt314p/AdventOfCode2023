#include <stdio.h>

#define BUFFER_COUNT 1024
#define true 1
#define false 0
#define bool int

int IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

void PartOne()
{
    char buffer[BUFFER_COUNT];
    unsigned int sum = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int gameNumber = 1;
    int invalidGamesSum = 0;

    int currentNumber = 0;
    bool readyToCheckColor = false;
    bool alreadyAccountedForGame = false;

    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '\n')
            {
                gameNumber++;
                alreadyAccountedForGame = false;
            }

            // skip checking if the current game is already marked as invalid
            if (alreadyAccountedForGame) continue;

            if (readyToCheckColor)
            {   
                // only check the first letter of the color
                if (c == 'r' && currentNumber > 12 ||
                    c == 'g' && currentNumber > 13 ||
                    c == 'b' && currentNumber > 14)
                {
                    invalidGamesSum += gameNumber;
                    alreadyAccountedForGame = true; // mark the current game as invalid
                }
                readyToCheckColor = false;
                continue;
            }

            if (!IsDigit(c))
            {
                if (c == ' ')
                {
                    if (currentNumber <= 12 || currentNumber == 0) continue;
                    // the number is greater than 12
                    
                    readyToCheckColor = true; 
                    continue;
                }
                currentNumber = 0;
                continue;
            }

            int value = c - '0';

            // the currentNumber already has a digit, shift to 10s place
            if (currentNumber != 0) currentNumber *= 10;

            currentNumber += value;
        }
    }

    // the total sum of games is 5050
    // so the valid games sum = total sum - invalid games sum
    printf("Sum: %d", 5050 - invalidGamesSum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];
    unsigned int sum = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int validGamesSum = 0;

    int currentNumber = 0;
    bool readyToCheckColor = false;

    // rgb
    int mins[] = { 0, 0, 0 };

    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '\n') // end of game, compute sum and reset minimums
            {
                validGamesSum += mins[0] * mins[1] * mins[2];
                mins[0] = mins[1] = mins[2] = 0;
            }

            if (readyToCheckColor)
            {
                int i = 0;
                if (c == 'g') i = 1;
                if (c == 'b') i = 2;

                // check if the current count is a new max
                if (currentNumber > mins[i]) mins[i] = currentNumber;
                currentNumber = 0;
                readyToCheckColor = false;
                continue;
            }

            if (!IsDigit(c))
            {
                if (c == ' ')
                {
                    if (currentNumber == 0) continue;
                    readyToCheckColor = true;
                    continue;
                }
                currentNumber = 0;
                continue;
            }

            int value = c - '0';

            if (currentNumber != 0) currentNumber *= 10;
            currentNumber += value;
        }
    }

    printf("Sum: %d", validGamesSum);
}

int main()
{
    //PartOne();
    PartTwo();
}