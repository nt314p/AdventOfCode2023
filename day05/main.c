#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_COUNT 1024
#define SEED_COUNT 20
#define MAP_COUNT 7

// every map has less than 50 map ranges
// avoid dynamic allocation
#define RANGES_PER_MAP 50
#define UINTS_PER_MAP (3 * RANGES_PER_MAP)

typedef struct MapRange
{
    unsigned int destStart;
    unsigned int srcStart;
    unsigned int length;
} MapRange;

// Sorts a single map
void SortMap(MapRange* map)
{
    int length = 0;
    for (; length < RANGES_PER_MAP; length++)
    {
        if (map[length].length == 0) break;
    }

    // Insertion sort... yes, it is O(n^2)
    for (int i = 0; i < length - 1; i++)
    {
        int lowestIndex = i;
        unsigned int lowestSrcStart = map[i].srcStart;

        for (int j = i + 1; j < length; j++)
        {
            MapRange m = map[j];

            if (m.srcStart < lowestSrcStart)
            {
                lowestIndex = j;
                lowestSrcStart = m.srcStart;
            }
        }

        if (i == lowestIndex) continue;

        MapRange temp = map[i];
        map[i] = map[lowestIndex];
        map[lowestIndex] = temp;
    }
}

// Maps a source number to a destination number, using the map at mapIndex
int DestNumber(MapRange* maps, int srcNumber, int mapIndex)
{
    int mapRangeIndex = mapIndex * RANGES_PER_MAP;

    for (int i = mapRangeIndex;; i++)
    {
        MapRange m = maps[i];
        if (m.length == 0) break; // end of map ranges for this map index

        if (m.srcStart > srcNumber) break;

        unsigned int diff = srcNumber - m.srcStart;

        if (diff <= m.length) // srcNumber is within the mapped range
        {
            return m.destStart + diff; // compute destination map
        }
    }

    return srcNumber; // number was not mapped
}

void PartOne()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    unsigned int seeds[SEED_COUNT] = { 0 };
    int seedsIndex = 0;

    // linear representation of all maps
    MapRange maps[RANGES_PER_MAP * MAP_COUNT] = { 0 };
    unsigned int* mapsUint32 = (unsigned int*)maps; // shhh
    int mapsIndex = 0;

    unsigned int currentNumber = -1;
    int isParsingSeeds = 1;

    // Parse all seeds and ranges
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                if (currentNumber == -1) currentNumber = 0;
                currentNumber *= 10;
                currentNumber += c - '0';
                continue;
            }

            if (c == ' ' || c == '\n')
            {
                if (currentNumber == -1) continue;

                if (isParsingSeeds)
                {
                    seeds[seedsIndex] = currentNumber;
                    seedsIndex++;
                }
                else
                {
                    mapsUint32[mapsIndex] = currentNumber;
                    mapsIndex++;
                }

                currentNumber = -1;

                if (seedsIndex == SEED_COUNT) isParsingSeeds = 0;
            }

            // we've reached the next map
            // increment the map index to the next map (in blocks of 150)
            if (c == ':' && mapsIndex != 0)
            {
                mapsIndex = UINTS_PER_MAP * (mapsIndex / UINTS_PER_MAP + 1);
            }
        }
    }

    for (int i = 0; i < sizeof(maps) / sizeof(MapRange); i += RANGES_PER_MAP)
    {
        SortMap(maps + i);
    }

    unsigned int minimumLocation = UINT32_MAX;

    for (int i = 0; i < SEED_COUNT; i++)
    {
        int srcNumber = seeds[i];

        for (int mapIndex = 0; mapIndex < MAP_COUNT; mapIndex++)
        {
            srcNumber = DestNumber(maps, srcNumber, mapIndex);
        }

        if (srcNumber < minimumLocation) minimumLocation = srcNumber;
    }

    printf("Min location: %u\n", minimumLocation);

    unsigned int n = 3763992295;
    for (int mapIndex = 0; mapIndex < MAP_COUNT; mapIndex++)
    {
        n = DestNumber(maps, n, mapIndex);
    }
    printf("Loc %u\n", n);
}

void PartTwo()
{
    char buffer[BUFFER_COUNT];

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
    }

    // linear representation of all maps
    MapRange maps[RANGES_PER_MAP * (MAP_COUNT + 1)] = { 0 };
    unsigned int* mapsUint32 = (unsigned int*)maps; // shhh
    int mapsIndex = 0;

    int mapSize = 400 * sizeof(MapRange);
    MapRange* srcMaps = malloc(mapSize);
    int srcMapsIndex = 0;
    MapRange* destMaps = malloc(mapSize);
    int destMapsIndex = 0;

    memset(srcMaps, 0, mapSize);
    memset(destMaps, 0, mapSize);

    unsigned int currentNumber = -1;
    int isParsingSeeds = 1;

    // Parse seed ranges and map ranges
    while (fgets(buffer, BUFFER_COUNT, fp) != NULL)
    {
        for (int index = 0; index < BUFFER_COUNT; index++)
        {
            char c = buffer[index];
            if (c == '\0') break;

            if (c >= '0' && c <= '9')
            {
                if (currentNumber == -1) currentNumber = 0;
                currentNumber *= 10;
                currentNumber += c - '0';
                continue;
            }

            if (c == ' ' || c == '\n')
            {
                if (currentNumber == -1) continue;

                mapsUint32[mapsIndex] = currentNumber;
                mapsIndex++;

                // For seeds, the srcStart is also the destStart
                if (isParsingSeeds && mapsIndex % 3 == 1)
                {
                    mapsUint32[mapsIndex] = currentNumber;
                    mapsIndex++;
                }

                currentNumber = -1;
            }

            // we've reached the next map
            // increment the map index to the next map (in blocks of 150)
            if (c == ':' && mapsIndex != 0)
            {
                mapsIndex = UINTS_PER_MAP * (mapsIndex / UINTS_PER_MAP + 1);
                isParsingSeeds = 0;
            }
        }
    }

    for (int i = 0; i < sizeof(maps) / sizeof(MapRange); i += RANGES_PER_MAP)
    {
        SortMap(maps + i);
    }

    // for (int i = 0; i < RANGES_PER_MAP * (MAP_COUNT + 1); i++)
    // {
    //     MapRange m = maps[i];
    //     if (m.length == 0)
    //     {
    //         printf(" ");
    //         continue;
    //     }
    //     printf("\ns: %u, d: %u, l: %u", m.srcStart, m.destStart, m.length);
    // }

    // Initialize our source map
    for (srcMapsIndex = 0;; srcMapsIndex++)
    {
        MapRange m = maps[srcMapsIndex];
        if (m.length == 0) break;
        srcMaps[srcMapsIndex] = m;
    }

    // Every iteration, we apply the map at index i to the
    // map ranges stored in srcMaps
    for (int mapIndex = 1; mapIndex < MAP_COUNT + 1; mapIndex++)
    {
        MapRange* currentMap = maps + mapIndex * RANGES_PER_MAP;
        destMapsIndex = 0;

        for (srcMapsIndex = 0;; srcMapsIndex++)
        {
            MapRange srcM = srcMaps[srcMapsIndex];
            if (srcM.length == 0) break;

            //printf("\nRemapping s: %u, d: %u, l: %u\n", srcM.srcStart, srcM.destStart, srcM.length);

            // the destination of the srcsrcMap is remapped if the destination range
            // falls within the current map range

            unsigned int offset = 0;

            int mapRangeIndex = 0;
            while (1)
            {
                MapRange m = currentMap[mapRangeIndex];

                // the next lowest unmapped dest value
                unsigned int currentDestValue = srcM.destStart + offset;
                unsigned int currentDestEnd = srcM.destStart + srcM.length - 1;
                const unsigned int currentSrcValue = srcM.srcStart + offset;
                unsigned int rangeLength;

                if (offset >= srcM.length)
                {
                    break; // Only happens when the last map range ran up against the
                    // src map upper bound
                }
                else if (m.length == 0)
                {
                    // no more maps
                    rangeLength = currentDestEnd - currentDestValue + 1;
                }
                else if (m.srcStart + m.length < srcM.destStart)
                {
                    // current map is below range
                    mapRangeIndex++;
                    continue;
                }
                else if (currentDestValue < m.srcStart)
                {
                    // current map is outside range
                    // OUT  (dest):  |-+--------------|
                    //  IN (m.src):          |------|
                    // currentDest value we are mapping isn't yet in the range
                    // of the current map range.
                    // perform a pass through map up to the start of the current
                    // map range.
                    rangeLength = __min(m.srcStart - currentDestValue, srcM.length);
                }
                else if (currentDestValue <= m.srcStart + m.length - 1)
                {
                    // current map is inside range
                    // OUT  (dest):  |-----------+----|
                    //  IN (m.src):          |------|
                    // currentDest value we are mapping is within the current
                    // map range range.
                    // ensure that the mapped range does not exceed the overall
                    // map range
                    unsigned int destEndValue = __min(
                        m.srcStart + m.length - 1, // end of map range
                        currentDestEnd
                    );

                    rangeLength = destEndValue - currentDestValue + 1;

                    // we might be mapping from the middle of a map range
                    // compute how far into the input we are
                    unsigned int destOffset = currentDestValue - m.srcStart;
                    currentDestValue = m.destStart + destOffset;

                    mapRangeIndex++; // also fetch the next map range, since we used this one
                }
                else
                {
                    abort();
                }

                MapRange n = { currentDestValue, currentSrcValue, rangeLength };
                destMaps[destMapsIndex] = n;
                destMapsIndex++;
                offset += n.length;
            }
        }

        // Swap source and destination maps
        MapRange* temp = destMaps;
        destMaps = srcMaps;
        srcMaps = temp;
        memset(destMaps, 0, mapSize); // clear destination map
    }

    unsigned int lowestLocation = UINT32_MAX;

    for (int i = 0;; i++)
    {
        MapRange m = srcMaps[i];
        if (m.length == 0) break;

        if (m.destStart < lowestLocation) lowestLocation = m.destStart;
    }

    printf("Lowest location: %u\n", lowestLocation);
    fclose(fp);
}

int main()
{
    //PartOne();
    PartTwo();
    
    return 0;
}