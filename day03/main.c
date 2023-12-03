#include <stdio.h>
#include <string.h>

#define BUFFER_COUNT 1024
#define LINE_LEN 141
#define NUMBER_COUNT 1219

// Example: 123 would be stored as
/*
value = 123
startIndex = 4
endIndex = 6
counted = 0 (false)
*/
// Assuming the number starts at index 4
typedef struct Number
{
    int value;
    int startIndex;
    int endIndex;
    int counted;
} Number;

// Finds the index of the first number n such that
// endIndex <= n.endIndex
int FindEndIndex(Number* numbers, int endIndex)
{
    int start = 0;
    int end = NUMBER_COUNT;

    while (1) // The numbers array is sorted so we use binary search
    {
        if (end - start <= 1) break;

        int mid = (start + end) / 2;

        if (numbers[mid].endIndex < endIndex)
        {
            start = mid;
            continue;
        }

        end = mid;
    }

    return end;
}

void PartOne()
{
    char buffer[BUFFER_COUNT];

    Number numbers[NUMBER_COUNT];
    memset(numbers, 0, NUMBER_COUNT * sizeof(Number));
    int numberIndex = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int currentNumberValue = 0;
    Number currentNumber = { 0 };

    int globalIndex = 0;

    // First pass, parse all numbers
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++, globalIndex++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                if (currentNumberValue == 0) currentNumber.startIndex = globalIndex;
                currentNumberValue *= 10;
                currentNumberValue += c - '0';
                continue;
            }

            if (currentNumberValue != 0) {
                currentNumber.endIndex = globalIndex - 1;
                currentNumber.value = currentNumberValue;

                numbers[numberIndex] = currentNumber;
                numberIndex++;
            }

            currentNumberValue = 0;
        }
    }

    fseek(fp, 0, SEEK_SET);

    globalIndex = 0;
    int sum = 0;

    // Second, find all symbols
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++, globalIndex++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c == '.' || c == '\n' || (c >= '0' && c <= '9')) continue;

            // these define indices on the left and right of the found symbol
            int leftIndex = globalIndex - LINE_LEN - 1;
            int rightIndex = leftIndex + 2;

            // Every iteration, we increase by LINE_LEN to index into the line below
            for (int i = 0; i < 3; i++, leftIndex += LINE_LEN, rightIndex += LINE_LEN)
            {
                // index into the numbers array
                // this number is the first number to have an endIndex >= leftIndex
                // this finds the lowest possible number that could be counted
                int numbersEndIndex = FindEndIndex(numbers, leftIndex);

                // move along the same line rightward, looking for more countable numbers
                for (int j = numbersEndIndex; j < NUMBER_COUNT; j++)
                {
                    Number n = numbers[j];

                    if (n.counted) continue;
                    //      EEEEEEE...
                    //...SSSSSS
                    //      LMR

                    // A number is only counted if it is within the bounds of the symbol
                    if (leftIndex <= n.endIndex && n.startIndex <= rightIndex)
                    {
                        sum += n.value;
                        n.counted = 1; // mark the number as counted
                        continue;
                    }

                    break; // No more valid numbers on this line, move to the next line
                }               
            }
        }
    }

    printf("Sum: %d\n", sum);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    Number numbers[NUMBER_COUNT];
    memset(numbers, 0, NUMBER_COUNT * sizeof(Number));
    int numberIndex = 0;

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    int currentNumberValue = 0;
    Number currentNumber = { 0 };

    int globalIndex = 0;

    // First pass, parse all numbers
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++, globalIndex++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                if (currentNumberValue == 0) currentNumber.startIndex = globalIndex;
                currentNumberValue *= 10;
                currentNumberValue += c - '0';
                continue;
            }

            if (currentNumberValue != 0) {
                currentNumber.endIndex = globalIndex - 1;
                currentNumber.value = currentNumberValue;

                numbers[numberIndex] = currentNumber;
                numberIndex++;
            }

            currentNumberValue = 0;
        }
    }

    fseek(fp, 0, SEEK_SET);

    globalIndex = 0;
    int sum = 0;

    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++, globalIndex++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c != '*') continue;

            int leftIndex = globalIndex - LINE_LEN - 1;
            int rightIndex = leftIndex + 2;

            int product = 1;
            int foundNumbers = 0; // number of gears, must be two to count

            for (int i = 0; i < 3; i++, leftIndex += LINE_LEN, rightIndex += LINE_LEN)
            {
                int numbersEndIndex = FindEndIndex(numbers, leftIndex);

                for (int j = numbersEndIndex; j < NUMBER_COUNT; j++)
                {
                    Number n = numbers[j];

                    if (leftIndex <= n.endIndex && n.startIndex <= rightIndex)
                    {
                        product *= n.value;
                        foundNumbers++;
                        continue;
                    }

                    break;
                }               
            }

            if (foundNumbers == 2) sum += product;
        }
    }

    printf("Sum: %d\n", sum);
}

int main()
{
    PartOne();
    PartTwo();

    return 0;
}