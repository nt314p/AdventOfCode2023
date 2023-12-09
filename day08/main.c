#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_COUNT 1024
#define DIRECTIONS_COUNT 269
#define NODE_COUNT 17576

typedef struct Node
{
    uint16_t left;
    uint16_t right;
    uint16_t visited;
} Node;

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    char directions[DIRECTIONS_COUNT];
    int directionsIndex = 0;

    // 26 * 26 * 26 = 17576

    Node nodes[NODE_COUNT];
    memset(nodes, 0, sizeof(nodes));
    int nodesIndex = 0;
    int currentValue = -1;

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (directionsIndex < DIRECTIONS_COUNT)
            {
                directions[directionsIndex] = c;
                directionsIndex++;
                continue;
            }

            if (c >= 'A' && c <= 'Z')
            {
                if (currentValue == -1) currentValue = 0;
                currentValue *= 26;
                currentValue += c - 'A';
            }

            if (c == '=') // parse 
            {
                nodesIndex = currentValue;
                currentValue = -1;
            }

            if (c == ',')
            {
                nodes[nodesIndex].left = currentValue;
                currentValue = -1;
            }

            if (c == ')')
            {
                nodes[nodesIndex].right = currentValue;
                currentValue = -1;
            }
        }
    }

    int count = 0;
    directionsIndex = 0;
    nodesIndex = 0;

    while (1)
    {
        if (nodesIndex == NODE_COUNT - 1) break;
        Node n = nodes[nodesIndex];
        nodesIndex = directions[directionsIndex] == 'L' ? n.left : n.right;
        count++;
        directionsIndex++;

        if (directionsIndex == DIRECTIONS_COUNT) directionsIndex = 0;
    }

    printf("Count: %d\n", count);
}

int main()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
    }

    char directions[DIRECTIONS_COUNT];
    int directionsIndex = 0;

    // 26 * 26 * 26 = 17576

    Node nodes[NODE_COUNT];
    memset(nodes, 0, sizeof(nodes));
    int nodesIndex = 0;
    int currentValue = -1;

    int startingNodes[6]; // there are 6 nodes that end with A
    int endingNodes[6]; // there are also 6 nodes that end with Z
    int startingNodesIndex = 0;

    unsigned long finishLength[6]; // steps from XXA to XXZ
    unsigned long cycleLength[6]; // steps from XXZ to XXZ

    while (fgets(buffer, BUFFER_COUNT, fp))
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (directionsIndex < DIRECTIONS_COUNT)
            {
                directions[directionsIndex] = c;
                directionsIndex++;
                continue;
            }

            if (c >= 'A' && c <= 'Z')
            {
                if (currentValue == -1) currentValue = 0;
                currentValue *= 26;
                currentValue += c - 'A';
            }

            if (c == '=') // parse 
            {
                nodesIndex = currentValue;
                currentValue = -1;
            }

            if (c == ',')
            {
                nodes[nodesIndex].left = currentValue;
                currentValue = -1;
            }

            if (c == ')')
            {
                nodes[nodesIndex].right = currentValue;
                currentValue = -1;

                if (nodesIndex % 26 == 0)
                {
                    startingNodes[startingNodesIndex] = nodesIndex;
                    startingNodesIndex++;
                }
            }
        }
    }

    unsigned long count = 0;
    directionsIndex = 0;
    nodesIndex = 0;

    int numberUnvisited = 0;

    for (int i = 0; i < 6; i++) // Find the finish length
    {
        int startingIndex = startingNodes[i];
        nodesIndex = startingIndex;
        directionsIndex = 0;
        count = 0;

        numberUnvisited = 0;

        int startingUnvisited = 0;

        while (1)
        {
            Node n = nodes[nodesIndex];
            (nodes + nodesIndex)->visited = 1;

            if (nodesIndex % 26 == 25) break;

            nodesIndex = directions[directionsIndex] == 'L' ? n.left : n.right;
            count++;
            directionsIndex++;

            if (directionsIndex == DIRECTIONS_COUNT) directionsIndex = 0;
        }

        finishLength[i] = count;
        endingNodes[i] = nodesIndex;
        printf("Index: %d, finish length: %lu, end: %d\n", i, count, nodesIndex);
    }

    printf("\n");

    for (int i = 0; i < 6; i++) // Find the cycle length
    {
        int startingIndex = endingNodes[i];
        nodesIndex = startingIndex;
        // if the steps to finish was 10, the last direction processed
        // was at index 9, so the next direction is at index 10
        directionsIndex = finishLength[i] % DIRECTIONS_COUNT;
        int startingDirectionsIndex = directionsIndex;
        count = 0;

        while (1)
        {
            Node n = nodes[nodesIndex];

            if (nodesIndex % 26 == 25 &&
                directionsIndex == startingDirectionsIndex &&
                count != 0) break;

            nodesIndex = directions[directionsIndex] == 'L' ? n.left : n.right;
            count++;
            directionsIndex++;

            //printf("Node index: %d\n", nodesIndex);

            if (directionsIndex == DIRECTIONS_COUNT) directionsIndex = 0;
        }

        cycleLength[i] = count;
        printf("Index: %d, cycle length: %lu, end: %d\n", i, count, nodesIndex);
    }

    unsigned long long steps[6];

    for (int i = 0; i < 6; i++)
    {
        steps[i] = finishLength[i];
    }

    while (1)
    {
        unsigned long long currStep = steps[0];

        int allLarger = 1;
        int allSame = 1;

        for (int i = 1; i < 6; i++)
        {
            if (currStep != steps[i]) allSame = 0;
            if (steps[i] < currStep) 
            {
                steps[i] += cycleLength[i];
                allLarger = 0;
            }
        }

        if (allSame) break;

        if (allLarger) steps[0] += cycleLength[0];
    }

    printf("Steps: %llu\n", steps[0]);

    return 0;
}