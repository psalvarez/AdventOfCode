#ifndef PART2_H
#define PART2_H

#include "Common.h"

#define ARRAY_SIZE (GRID_SIZE * GRID_SIZE)

typedef enum Error
{
    NO_ERROR,
    BRIGHTNESS_OVERFLOW
} Error;

int countBrightness(unsigned char* grid);
void drawLight(int row, int column, unsigned char* grid);
Error changeLight(unsigned char* grid, int row, int column, enum LightChange action);

#endif // PART2_H