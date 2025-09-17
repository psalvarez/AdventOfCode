#if !defined(PART1_H)
#define PART1_H

#define BITS_PER_BYTE 8
#define ARRAY_SIZE (GRID_SIZE * (GRID_SIZE / BITS_PER_BYTE))

#include "Common.h"

typedef struct LightCoord
{
    int ArrayIdx;
    int BitPosition;
} LightCoord;

LightCoord coordsToArrayIdx(int row, int column);
int countLit(unsigned char* grid);
void drawLight(int row, int column, unsigned char* grid);
 changeLight(unsigned char* grid, int row, int column, enum LightChange action);

#endif // PART1_H