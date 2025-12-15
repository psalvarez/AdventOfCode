#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void update(const int newValue, int* digits, int idx, const int max)
{
    if (idx >= max)
        return;

    // If it is equal, we still need to check if we can replace the units
    if(newValue >= digits[idx])
    {
        update(digits[idx], digits, idx + 1, max);
        digits[idx] = newValue;
    }
}

int main()
{
    char inBuffer[500];
    long long total = 0;
    int lineN = 0;
    while (fgets(inBuffer, sizeof(inBuffer), stdin) != NULL)
    {
        size_t inLen = strlen(inBuffer) - 1; // We remove the trailing '\n'
        printf("%d > %s", lineN, inBuffer);

        const int nDigits = 12;
        int currentPos = inLen - nDigits - 1; // We start from checking from the n + 1 position from the left
        int digits[nDigits];
        for (int i = 0; i < nDigits; ++i)
        {
            digits[i] = inBuffer[currentPos + 1 + i] - '0';
        }

        while(currentPos >= 0)
        {
            // printf("%d - %d%d\n", currentPos, maxTens, maxUnits);
            int current = inBuffer[currentPos] - '0';
            update(current, digits, 0, nDigits);

            --currentPos;
        }

        // Debugging bit
        // Find position of the tens, make sure the units exist to the right
        // char tens = (char)maxTens + '0';
        // char units = (char)maxUnits + '0';
        // int tensPos = -1;
        // int unitsPos = -1;
        // for (size_t i = 0; i < inLen; ++i)
        // {
            // if (tensPos < 0)
            // {
                // if (tens == inBuffer[i])
                    // tensPos = i;
            // }
            // else if (units == inBuffer[i])
            // {
                // unitsPos = i;
                // break; // We are keeping the leftmost of each value
            // }
        // }

        // printf("Chars: %c%c; Positions: %d,%d\n", tens, units, tensPos, unitsPos);
        // if (tensPos > unitsPos)
        // {
            // printf("ERROR");
            // return -1;
        // }

        long long fullNum = 0;
        for (int i = 0; i < nDigits; ++i)
        {
            fullNum += digits[i] * pow((double)10, (double)(nDigits - 1 - i));
        }

        printf("%llu\n", fullNum);
        total += fullNum;
        if (total < 0)
        {
            printf("int overflow");
            return -1;
        }

        ++lineN;
    }

    // Part 1 numbers:
    // >17143
    // <17345
    // Input is 200 lines, max number in single line is 99, max total is 19800 (200 * 99)
    printf("Total %llu\n", total);

}
