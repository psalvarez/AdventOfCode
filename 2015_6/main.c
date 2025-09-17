#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"

#define PART_1 0

#if PART_1
#include "Part1.h"
#else
#include "Part2.h"
#endif

#pragma warning(disable: 5045)  // Spectre mitigation warning


int changeLights(unsigned char* grid, int upLeftX, int upLeftY, int downRightX, int downRightY, enum LightChange action)
{
    for (int row = upLeftY; row <= downRightY; ++row)
    {
        // Potential optimisation: Make masks on the full char for ranges
        for (int column = upLeftX; column <= downRightX; ++column)
        {
            Error err = changeLight(grid, row, column, action);
            if (err != NO_ERROR)
            {
                printf("ERROR: %d at (%d, %d)\n", err, row, column);
                return 1;
            }
        }
    }
    return 0;
}

#if PART_1
void drawGrid(unsigned char* grid, int size)
{
    for (int row = 0; row < size; ++row)
    {
        for (int column = 0; column < size; ++column)
        {
            drawLight(row, column, grid);
        }
        printf("\n");
    }
}
#endif

int main(int argc, char* argv[])
{
    // Each light is 1 bit. Sorted MSB to LSB (natural ordering). Interleaving it on a single array
    if (argc < 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    // Let's be brave and treat chars as ints on part 2
    // Will just have to check that none of them goes beyond 255.
    unsigned char grid[ARRAY_SIZE];
    for (int i  = 0; i < ARRAY_SIZE; ++i)
    {
        grid[i] = 0;
    }

    changeLights(grid, 0, 0, GRID_SIZE - 1, GRID_SIZE - 1, OFF); // Initialise array
    // drawGrid(grid, 16);
    printf("\n----------------\n");

    const char* fileName = argv[1];
    FILE* file = fopen(fileName, "rb");
    char line[64];

    while(fgets(line, sizeof(line), file))
    {
        char command[32];
        int x = -1;
        int y = -1;
        int z = -1;
        int t = -1;

        char onOff[8];
        int read = sscanf_s(line, "%s %d,%d through %d, %d", command, (unsigned int) sizeof(command), &x, &y, &z, &t);
        enum LightChange mode;

        if (read > 1) // This is a TOGGLE command
        {
            mode = TOGGLE;
        }
        else
        {
            // Retry reading as turn
            read = sscanf_s(line, "%s %s %d,%d through %d, %d", command, (unsigned int) sizeof(command), onOff, (unsigned int) sizeof(onOff), &x, &y, &z, &t);
            if (read > 2)
            {
                // Pretty unsafe, but will do the job
                mode = strcmp(onOff, "on") == 0 ? ON : OFF;
            }
            else
            {
                printf("ERROR: couldn't read command");
                return 1;
            }
        }

        printf("%s, %d,%d; %d,%d\n", command, x, y, z, t);
        int err = changeLights(grid, x, y, z, t, mode);
        if (err != 0)
        {
            return 1;
        }
        // drawGrid(grid, 16);
        printf("\n----------------\n");
    }

    fclose(file);

#if PART_1
    int result = countLit(grid);
    printf("There are %d lights on", result);
#else
    int result = countBrightness(grid);
    printf("The total brightness is %d", result);
#endif

#if TEST_DRAW

    changeLights(grid, 0, 0, GRID_SIZE-1, GRID_SIZE-1, OFF);
    drawGrid(grid, 16);
    printf("\n----------------\n");
    changeLights(grid, 0, 0, 5, 5, ON);
    drawGrid(grid, 16);
    printf("\n----------------\n");
    changeLights(grid, 3, 3, 6, 6, TOGGLE);
    drawGrid(grid, 16);
    printf("\n----------------\n");
#endif
}
