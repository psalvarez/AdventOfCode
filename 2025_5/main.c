#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_RANGES = 11234494753753851930u; // Number of all the elements in all the ranges in Input.txt, not deduping values (measured from pt 1). 
typedef struct Range
{
    size_t min;
    size_t max;
} Range;

typedef struct RangeElm RangeElm;
struct RangeElm
{
    Range r;
    RangeElm* prev;
    RangeElm* next;
};

int inRange(size_t value, Range* range)
{
    if (value >= range->min && value <= range->max)
        return 1;
    return 0; // 0 no, 1 yes
}

void addToFresh(size_t newValue, size_t* allFreshIds, size_t* totalFreshIds)
{
    size_t i = 0;
    while(i < *totalFreshIds)
    {
        if (newValue == allFreshIds[i])
        {
            ++i;
            continue;
        }
        break;
    }

    if (i >= *totalFreshIds)
    {
        allFreshIds[*totalFreshIds] = newValue;
        ++(*totalFreshIds);
    }
}

int canMerge(Range* r1, Range* r2)
{
    // Can merge if at least one of the edges of one range is contained within the ranges of the
    // other.
    if (inRange(r1->min, r2) == 1 ||
        inRange(r1->max, r2) == 1 ||
        inRange(r2->min, r1) == 1 ||
        inRange(r2->max, r1) == 1)
    {
        return 1;
    }

    return 0;
}

int merge(RangeElm* r1, RangeElm* r2) // Return int here and avoid canMerge?
{
    // printf("merging... ");
    int result = 0;

    // r1 is right
    if (r1->r.max > r2->r.max && inRange(r1->r.min, &r2->r))
    {
        r1->r.min = r2->r.min;
        result = 1;
    }
    // r1 is left
    else if (r1->r.min < r2->r.min && inRange(r1->r.max, &r2->r))
    {
        r1->r.max = r2->r.max;
        result = 1;
    }
    // r1 is subset
    else if (inRange(r1->r.max, &r2->r) && inRange(r1->r.min, &r2->r))
    {
        r1->r.min = r2->r.min;
        r1->r.max = r2->r.max;
        result = 1;
    }
    // r1 is superset (r2 is subset)
    else if (inRange(r2->r.max, &r1->r) && inRange(r2->r.min, &r1->r))
    {
        result = 1;
    }

    if (result == 1)
    {
        if (r2->next != NULL)
            r2->next->prev = r2->prev;
        if (r2->prev != NULL)
            r2->prev->next = r2->next;
        free(r2);
    }

    return result;
}

int main()
{
    // NEXT UP: the merging algorithm works in the test case scenario. I've changed the test list and it breaks - fix that.
    char inBuffer[50];
    printf("Ranges:\n");
    Range ranges[200];
    size_t rangesLen = 0;

    int parsingRanges = 1;
    size_t totalFresh = 0;
    RangeElm* prevElm = NULL;
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
            RangeElm* newElm = malloc(sizeof(RangeElm));
            newElm->prev = NULL;
            newElm->next = NULL;
            int result = sscanf(inBuffer, "%lu-%lu", &newElm->r.min, &newElm->r.max); 
            result = sscanf(inBuffer, "%lu-%lu", &ranges[rangesLen].min, &ranges[rangesLen].max);  // Keep this for part 1
            if (result != 2)
            {
                printf("Error %d\n", result);
            }
            printf("Range: %s -> %zu-%zu\n", inBuffer, newElm->r.min, newElm->r.max);
            if (prevElm != NULL)
                prevElm->next = newElm;

            newElm->prev = prevElm;
            prevElm = newElm;

            ++rangesLen;
        }
        else
        {
            int result = 0;
            size_t i = 0;
            size_t testValue = 0;
            sscanf(inBuffer, "%zu", &testValue);
            // printf("testValue %zu\n", testValue);
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

    printf("\nPart 2 begins\n");
    RangeElm* const lastElm = prevElm;
    RangeElm* rightmost = lastElm;
    int i = 0;
    while (rightmost != NULL && i < 10)
    {
        RangeElm* currentElm = rightmost->prev;
        if (currentElm == NULL)
        {
            printf("Reached end of list, breaking\n");
            break;
        }

        // printf("Rightmost %zu-%zu; Current %zu-%zu\n", rightmost->r.min, rightmost->r.max, currentElm->r.min, currentElm->r.max);
        int merged = 0;
        while (currentElm != NULL)
        {
            /*
            if (currentElm->prev != NULL)
            {
                printf("%zu-%zu > ", currentElm->prev->r.min, currentElm->prev->r.max);
            }

            printf("%zu-%zu", currentElm->r.min, currentElm->r.max);

            if (currentElm->next != NULL)
            {
                printf(" >> %zu-%zu", currentElm->next->r.min, currentElm->next->r.max);
            }

            printf("\n");
            */
            // printf("Attempting %zu-%zu", rightmost->r.min, rightmost->r.max);
            // printf(" and %zu-%zu merge... ", currentElm->r.min, currentElm->r.max);
            merged = merge(rightmost, currentElm);
            if (merged == 1)
            {
                printf("merged\n");
                break; 
            }
            else
            {
                // printf("not merged\n");
                currentElm = currentElm->prev;
            }
        }

        printf("Setting rightmost\n");
        if (merged == 0)
        {
            rightmost = rightmost->prev;
        }
        else
        {
            rightmost = lastElm;
        }
    }

    // Ranges check
    size_t totalFreshIds = 0;
    RangeElm* currentElm = lastElm;
    while (currentElm != NULL)
    {
        printf("%zu-%zu; ", currentElm->r.min, currentElm->r.max);
        totalFreshIds += currentElm->r.max - currentElm->r.min + 1; // +1 to account for inclusive ranges
        currentElm = currentElm->prev;
    }
    printf("\n");

    printf("(Part 1) Total fresh available %lu\n", totalFresh);
    printf("(Part 2) Total fresh ids %lu\n", totalFreshIds);
}
