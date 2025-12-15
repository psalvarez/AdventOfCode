#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main()
{
    char inBuffer[500];
    int total = 0;
    int lineN = 0;
    while (fgets(inBuffer, sizeof(inBuffer), stdin) != NULL)
    {
        size_t inLen = strlen(inBuffer) - 1; // We remove the trailing '\n'
        printf("%d > %s", lineN, inBuffer);

        int currentPos = inLen - 3; // We start from checking from the third from the left
        int maxTens = inBuffer[currentPos + 1] - '0'; // Second from the left
        int maxUnits = inBuffer[currentPos + 2] - '0'; // Last in the sequence
        while(currentPos >= 0)
        {
            // printf("%d - %d%d\n", currentPos, maxTens, maxUnits);
            int current = inBuffer[currentPos] - '0';

            // If it is equal, we still need to check if we can replace the units
            if(current >= maxTens)
            {
                if (maxTens > maxUnits)
                    maxUnits = maxTens;
                maxTens = current;
            }

            --currentPos;
        }

        // Debugging bit
        // Find position of the tens, make sure the units exist to the right
        char tens = (char)maxTens + '0';
        char units = (char)maxUnits + '0';
        int tensPos = -1;
        int unitsPos = -1;
        for (size_t i = 0; i < inLen; ++i)
        {
            if (tensPos < 0) 
            {
                if (tens == inBuffer[i])
                    tensPos = i;
            }
            else if (units == inBuffer[i])
            {
                unitsPos = i;
                break; // We are keeping the leftmost of each value
            }
        }

        printf("Chars: %c%c; Positions: %d,%d\n", tens, units, tensPos, unitsPos);
        if (tensPos > unitsPos)
        {
            printf("ERROR");
            return -1;
        }

        int fullNum = maxTens * 10 + maxUnits;
        printf("%d\n", fullNum);
        total += fullNum;
        ++lineN;
    }

    // >17143
    // <17345
    // Input is 200 lines, max number in single line is 99, max total is 19800 (200 * 99)
    printf("Total %d\n", total);
}
