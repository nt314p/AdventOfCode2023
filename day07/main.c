#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_COUNT 1024
#define HAND_COUNT 1000

typedef struct Hand
{
    uint16_t bet;
    uint8_t cards[5];
    uint8_t strength;
} Hand;

// five of a kind   6
// four of a kind   5
// full house       4
// three of a kind  3
// two pair         2
// one pair         1
// high card        0

int GetStrength(int* duplicates)
{
    int three = 0;
    int pair = 0;

    for (int i = 0; i < 13; i++)
    {
        int count = duplicates[i];
        if (count == 5) return 6; // five of a kind
        if (count == 4) return 5; // four of a kind
        if (count == 3) three = 1; // either three of a kind or full house
        if (count == 2)
        {
            if (pair != 0) return 2; // two pair
            pair = 1;
        }
    }

    if (three)
    {
        if (pair) return 4; // full house
        return 3; // three of a kind
    }

    if (pair) return 1; // one pair
    return 0; // high card
}

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
    }

    Hand hands[HAND_COUNT] = { 0 };
    int handsIndex = 0;
    uint64_t* longHands = (uint64_t*)hands;

    // index 0 -> 2
    // index 1 -> 3
    // ...
    // index 11 -> K (13)
    // index 12 -> A (14)
    int duplicates[13];
    memset(duplicates, 0, sizeof(duplicates));

    int cardsIndex = 0;
    int currentValue = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            unsigned char c = buffer[index];
            if (c == '\0') break;

            if (c == 'A') c = '0' + 14;
            if (c == 'K') c = '0' + 13;
            if (c == 'Q') c = '0' + 12;
            if (c == 'J') c = '0' + 11;
            if (c == 'T') c = '0' + 10;

            if (c == ' ') continue;
            if (c == '\n')
            {
                int strength = GetStrength(duplicates);

                hands[handsIndex].strength = strength;
                hands[handsIndex].bet = currentValue;
                handsIndex++;

                cardsIndex = 0;
                currentValue = 0;

                memset(duplicates, 0, sizeof(duplicates));
                continue;
            }

            if (cardsIndex < 5)
            {
                hands[handsIndex].cards[4 - cardsIndex] = c;
                cardsIndex++;
                duplicates[c - '0' - 2]++;
                continue;
            }

            currentValue *= 10;
            currentValue += c - '0';
        }
    }

    // Sort hands
    for (int i = 0; i < HAND_COUNT - 1; i++)
    {
        uint64_t lowest = longHands[i];
        int lowestIndex = i;

        for (int j = i + 1; j < HAND_COUNT; j++)
        {
            uint64_t hand = longHands[j];

            if (hand < lowest)
            {
                lowest = hand;
                lowestIndex = j;
            }
        }

        if (i == lowestIndex) continue;

        longHands[lowestIndex] = longHands[i];
        longHands[i] = lowest;
    }

    int sum = 0;

    for (int i = 0; i < HAND_COUNT; i++)
    {
        Hand h = hands[i];
        sum += h.bet * (i + 1);
    }

    printf("Sum: %d\n", sum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
    }

    Hand hands[HAND_COUNT] = { 0 };
    int handsIndex = 0;
    uint64_t* longHands = (uint64_t*)hands;

    // index 0 -> 2
    // index 1 -> 3
    // ...
    // index 11 -> K (13)
    // index 12 -> A (14)
    int duplicates[13];
    memset(duplicates, 0, sizeof(duplicates));

    int cardsIndex = 0;
    int currentValue = 0;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            unsigned char c = buffer[index];
            if (c == '\0') break;

            if (c == 'A') c = '0' + 14;
            if (c == 'K') c = '0' + 13;
            if (c == 'Q') c = '0' + 12;
            if (c == 'J') c = '0' + 11;
            if (c == 'T') c = '0' + 10;

            if (c == ' ') continue;
            if (c == '\n')
            {
                int jokerCount = duplicates[9];
                duplicates[9] = 0;

                int three = 0;
                int pair = 0;

                int largestCount = duplicates[0];
                int largestCountIndex = 0;

                for (int i = 1; i < 13; i++)
                {
                    if (duplicates[i] > largestCount)
                    {
                        largestCount = duplicates[i];
                        largestCountIndex = i;
                    }
                }

                duplicates[largestCountIndex] += jokerCount; // always worth to give to the largest
                
                int strength = GetStrength(duplicates);

                hands[handsIndex].strength = strength;
                hands[handsIndex].bet = currentValue;
                handsIndex++;

                cardsIndex = 0;
                currentValue = 0;

                memset(duplicates, 0, sizeof(duplicates));
                continue;
            }

            if (cardsIndex < 5)
            {
                // jokers are worse than 2
                hands[handsIndex].cards[4 - cardsIndex] = (c == '0' + 11) ? '1' : c;
                cardsIndex++;
                duplicates[c - '0' - 2]++;
                continue;
            }

            currentValue *= 10;
            currentValue += c - '0';
        }
    }

    // Sort hands
    for (int i = 0; i < HAND_COUNT - 1; i++)
    {
        uint64_t lowest = longHands[i];
        int lowestIndex = i;

        for (int j = i + 1; j < HAND_COUNT; j++)
        {
            uint64_t hand = longHands[j];

            if (hand < lowest)
            {
                lowest = hand;
                lowestIndex = j;
            }
        }

        if (i == lowestIndex) continue;

        longHands[lowestIndex] = longHands[i];
        longHands[i] = lowest;
    }

    int sum = 0;

    for (int i = 0; i < HAND_COUNT; i++)
    {
        Hand h = hands[i];
        sum += h.bet * (i + 1);
    }

    printf("Sum: %d\n", sum);
}

int main()
{
    //PartOne();
    PartTwo();

    return 0;
}