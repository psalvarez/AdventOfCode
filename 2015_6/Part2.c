#include "Part2.h"

int countBrightness(unsigned char* grid)
{
    int total = 0;
    for (int i  = 0; i < ARRAY_SIZE; ++i)
    {
        total += grid[i];
    }

    return total;
}

int coordsToArrayIdx(int row, int column)
{
    return row * GRID_SIZE + column;
}

Error changeLight(unsigned char* grid, int row, int column, enum LightChange action)
{
    int idx = coordsToArrayIdx(row, column);
    switch(action)
    {
    case ON:
        if (grid[idx] >= 255)
            return BRIGHTNESS_OVERFLOW;

        ++grid[idx];

        break;
    case OFF:
        if (grid[idx] > 0)
            --grid[idx];

        break;
    case TOGGLE:
        if (grid[idx] >= 255)
            return BRIGHTNESS_OVERFLOW;

        grid[idx] += 2;
        break;
    default:
        break;
    }

    return NO_ERROR;
}
