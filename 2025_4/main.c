#include "stdio.h"
#include "string.h"

#define ROW_SIZE 200

int isAccessible(char grid[ROW_SIZE][ROW_SIZE], int row, int column, int rowLen)
{
    int totalRolls = 0;

    if (grid[row][column] == '.')
        return 0;

    // printf("Processing %d,%d\n", row, column);
    for (int i = row -1; i <= row + 1; ++i)
    {
        // printf("i: %d\n", i);
        if (i < 0 || i > rowLen)
        {
            continue;
        }

        for (int j = column - 1; j <= column + 1; ++j)
        {
            // printf("j: %d\n", j);
            if ((j < 0 || j > rowLen) || // Out of bounds
                (i == row && j == column))        // Skip the roll itself
            {
                continue;
            }

            if (grid[i][j] == '@')
            {
                // printf("ROLL: %d,%d\n", i, j);
                // grid[i][j] = 'x';
                ++totalRolls;
            }
        }
    }
    // printf("Surrounding for %d,%d: %d\n", row, column, totalRolls);

    return totalRolls < 4 ? 1 : 0;
}

int main()
{
    char grid[ROW_SIZE][ROW_SIZE];
    char inBuffer[ROW_SIZE];
    
    // Bulk load first, then we'll process. We need multiple lines for processing a single item, so
    // may as well just load first.
    int rowTotal = 0;
    while (fgets(inBuffer, sizeof(inBuffer), stdin) != NULL)
    {
        inBuffer[strlen(inBuffer) - 1] = '\0'; // Replace new line character with null
        memcpy(grid[rowTotal],inBuffer,  ROW_SIZE);
        ++rowTotal;
    }
    
    // return process(grid, 0, 7, strlen(grid[0])); // Debug

    printf("\n");

    printf("Into processing\n");
    int accessible = 0;
    int totalAccessible = 0;
    int totalRemoved = 0; // Should probably be the same as totalAccessible?
    int accessibleCoords[ROW_SIZE * ROW_SIZE][2];

    do
    {
        printf("Removed %d\n", totalRemoved);
        accessible = 0;
        // Find currently accessible rolls
        for (int r = 0; r < rowTotal; ++r)
        {
            const int kRowLen = strlen(grid[r]);
            for (int c = 0; c < kRowLen; ++c)
            {
                int result =  isAccessible(grid, r, c, kRowLen);
                if (result == 1)
                {
                    // printf("ACC %d,%d\n", r, c);
                    accessibleCoords[accessible][0] = r;
                    accessibleCoords[accessible][1] = c;
                    ++accessible;
                }
            }
            totalAccessible += accessible;
        }

        // Move accessible rolls out
        for (int i = 0; i < accessible; ++i)
        {
            grid[accessibleCoords[i][0]][accessibleCoords[i][1]] = '.';
            ++totalRemoved;
        }
    }
    while (accessible > 0);

    for (size_t i = 0; i < strlen(grid[0]); ++i)
    {
        printf("%s\n", grid[i]);
    }

    printf("Total accessible rolls: %d\n", totalAccessible);
    printf("Total removed rolls: %d\n", totalRemoved);
}