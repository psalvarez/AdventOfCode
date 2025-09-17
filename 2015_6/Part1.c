#include "Part1.h"

#include <stdio.h>

LightCoord coordsToArrayIdx(int row, int column)
{
    LightCoord result;
    int columnGroup = column / BITS_PER_BYTE;
    const int rowSize = GRID_SIZE / BITS_PER_BYTE;

    result.ArrayIdx = row * rowSize + columnGroup;
    result.BitPosition = column % BITS_PER_BYTE;

    return result;
}

int countLit(unsigned char* grid)
{
    int totalLights = 0;
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        for (int j = 0; j < BITS_PER_BYTE; ++j)
        {
            totalLights += (unsigned int) (grid[i] & (1 << (7 - j))) > 0 ? 1 : 0;
        }
    }

    return totalLights;
}

void drawLight(int row, int column, unsigned char* grid)
{
    int columnGroup = column / BITS_PER_BYTE;
    int bitPosition = column % BITS_PER_BYTE;
    const int elmsPerRow = GRID_SIZE / BITS_PER_BYTE;

    int isOn = grid[row * elmsPerRow + columnGroup] & (1 << (7 - bitPosition));
    printf("%c ", isOn == 0 ? '.' : 'x');
}

void changeLight(unsigned char* grid, int row, int column, enum LightChange action)
{
    LightCoord light = coordsToArrayIdx(row, column);

    if (light.ArrayIdx < 0 || light.ArrayIdx >= ARRAY_SIZE)
    {
        printf("ERROR: Light array idx %d is bigger than array's size %d", light.ArrayIdx, ARRAY_SIZE);
        return;
    }
    
    switch (action)
    {
    case ON:
        grid[light.ArrayIdx] |= (1 << (7 - light.BitPosition));
        break;
    case OFF:
        grid[light.ArrayIdx] &= ~(1 << (7 - light.BitPosition));
        break;
    case TOGGLE:
        grid[light.ArrayIdx] ^= (1 << (7 - light.BitPosition));
        break;
    default:
        break;
    }
}
