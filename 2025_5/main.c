#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Range
{
    size_t min;
    size_t max;
} Range;

int inRange(size_t value, Range* range)
{
    if (value >= range->min && value <= range->max)
        return 1;
    return 0; // 0 no, 1 yes
}

int main()
{
    char inBuffer[50];
    printf("Ranges:\n");
    Range ranges[200];
    size_t rangesLen = 0;

    int parsingRanges = 1;
    size_t totalFresh = 0;
    while(fgets(inBuffer, sizeof(inBuffer), stdin) != NULL)
    {
        inBuffer[strlen(inBuffer) - 1] = '\0';
        
        if (inBuffer[0] == '\0')
        {
            parsingRanges = 0;
            printf("\nIds:\n");
            continue;
        }

        if (parsingRanges > 0)
        {
            int result = sscanf(inBuffer, "%lu-%lu", &ranges[rangesLen].min, &ranges[rangesLen].max); 
            if (result != 2)
            {
                printf("Error %d\n", result);
            }
            printf("Range: %s -> %zu-%zu\n", inBuffer, ranges[rangesLen].min, ranges[rangesLen].max);
            ++rangesLen;
        }
        else
        {
            int result = 0;
            size_t i = 0;
            size_t testValue = 0;
            sscanf(inBuffer, "%zu", &testValue);
            printf("testValue %zu\n", testValue);
            while (i < rangesLen)
            {
                result = inRange(testValue, &ranges[i]);
                if (result > 0)
                    break;
                ++i;
            }

            if(i >= rangesLen)
            {
                printf("%s not in range\n", inBuffer);
            }
            else
            {
                printf("%s -> %lu-%lu\n", inBuffer, ranges[i].min, ranges[i].max);
                ++totalFresh;
            }
        }
    }

    // != 0

    printf("Total fresh %lu\n", totalFresh);
}
