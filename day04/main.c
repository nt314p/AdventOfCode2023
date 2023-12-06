#include <stdio.h>
#include <string.h>

#define WINNING_NUM_COUNT 10
#define NUM_CARDS 198
#define BUFFER_COUNT 1024
// Each card contains 10 winning numbers
//            1  2  3  4  5  6  7  8  9 10
// Card XYZ: 97 78 45 39 22 24 56 81 12 70 |

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int sum = 0;

    int winningNumbers[WINNING_NUM_COUNT];
    int winningNumbersIndex = 0;
    int currentNumber = 0;

    int isOwnNumbers = 0;
    int points = 0;

    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
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

            if (c == ':') currentNumber = 0;

            if (c == '|') isOwnNumbers = 1;

            if ((c == ' ' || c == '\n') && currentNumber != 0)
            {
                if (isOwnNumbers == 0) // add winning number to list
                {
                    winningNumbers[winningNumbersIndex] = currentNumber;
                    winningNumbersIndex++;
                }
                else // check if our own number is winning
                {
                    for (int i = 0; i < WINNING_NUM_COUNT; i++)
                    {
                        if (currentNumber != winningNumbers[i]) continue;

                        if (points == 0)
                        {
                            points = 1;
                            continue;
                        }

                        points *= 2;
                    }

                }

                currentNumber = 0;
            }

            if (c == '\n')
            {
                sum += points;
                isOwnNumbers = 0;
                winningNumbersIndex = 0;
                points = 0;
            }
        }
    }

    printf("Sum: %d", sum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int sum = 0;

    int winningNumbers[WINNING_NUM_COUNT];
    int winningNumbersIndex = 0;
    int currentNumber = 0;

    int isOwnNumbers = 0;
    int numMatches = 0;

    // the ith index holds the number of winning matches for the (i+1)th card
    int cards[NUM_CARDS] = { 0 };
    int cardsIndex = 0;

    // the ith index holds the absolute number of cards that are processed
    // when the (i + 1)th card is processed
    // this includes processing the current card (minimum one)
    // also known as how many total cards are won after winning the card
    int cardsProcessedCount[NUM_CARDS];

    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
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

            if (c == ':') currentNumber = 0;
            if (c == '|') isOwnNumbers = 1;

            if ((c == ' ' || c == '\n') && currentNumber != 0)
            {
                if (isOwnNumbers == 0) // add winning number to list
                {
                    winningNumbers[winningNumbersIndex] = currentNumber;
                    winningNumbersIndex++;
                    //printf("Parsed win number: %d\n", currentNumber);
                }
                else // check if our own number is winning
                {
                    for (int i = 0; i < WINNING_NUM_COUNT; i++)
                    {
                        if (currentNumber != winningNumbers[i]) continue;
                        numMatches++;
                    }
                }

                currentNumber = 0;
            }

            if (c == '\n')
            {
                cards[cardsIndex] = numMatches;
                cardsIndex++;

                isOwnNumbers = 0;
                winningNumbersIndex = 0;
                numMatches = 0;
            }
        }
    }

    // iterate backwards
    for (int i = NUM_CARDS - 1; i >= 0; i--)
    {
        // we minimally process one card when we process the card
        int processedCount = 1;

        int numMatches = cards[i];

        // we iterate numMatches cards forward, and process them
        for (int j = 1; j < numMatches + 1; j++)
        {
            // we fetch how many cards are processed when we process the next card
            // and add it to how many cards we have process for the current card
            processedCount += cardsProcessedCount[i + j];
        }

        cardsProcessedCount[i] = processedCount;
    }

    // for (int i = 0; i < NUM_CARDS; i++)
    // {
    //     printf("Card: %d; matches: %d\n", (i + 1), cards[i]);
    // }

    for (int i = 0; i < NUM_CARDS; i++)
    {
        //printf("Card: %d; processed: %d\n", (i + 1), cardsProcessedCount[i]);
        sum += cardsProcessedCount[i];
    }

    printf("Sum: %d", sum);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}