#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>

typedef enum Bool
{
    FALSE, // 0
    TRUE   // 1
} Bool;

Bool isValidId(char* id)
{
    size_t size = strlen(id);

    size_t divisor = 1;
    // We divide the id into equal chunks progressively, and we compare each chunk with the first.
    // If all chunks are equal for one division, the string is invalid.
    while (divisor <= size)
    {
        ++divisor;
        if (size % divisor != 0)
        {
            continue;
        }
        // printf("divisor %lu\n", divisor);

        size_t chunkLen = size / divisor;
        size_t i = 1;
        // Check the string of length chunkLen for all chunks
        while (i < divisor)
        {
            if (strncmp(id, id + chunkLen * i, chunkLen) != 0)
            {
                break;
            }
            ++i;
        }

        if (i == divisor)
        {
            // printf("i == divisor - 1\n");
            return FALSE;
        }
    }

    return TRUE;
}

Bool isValidIdPt1(char* id)
{
    // Calculate the size, divide by 2. If first and second half are the same.
    int size = 0;
    while (id[size] != '\0')
    {
        ++size;
    }

    if (size % 2 != 0)
    {
        return TRUE; // If it can't be split in 2 equal substrings, it can't be repeated and hence can't be invalid
    }

    Bool result = FALSE;
    const int halfSize = size / 2;
    for (int i = 0; i < halfSize; ++i)
    {
        if (id[i] != id[i + halfSize])
        {
            result = TRUE;
            break;
        }
    }

    return result;
}

long unsigned int invalidIdSum(char* range)
{
    char* separator = range;
    while (*separator != '-')
    {
        ++separator;
    }

    char* lowerStr = range; 
    char* upperStr = separator + 1;
    *separator = '\0'; // How gross is doing this?

    int maxDigits = 0;
    while (upperStr[maxDigits] != '\0')
        ++maxDigits;

    printf("%s-%s; %d\n", lowerStr, upperStr, maxDigits);
    long unsigned int lower = atol(lowerStr);
    long unsigned int upper = atol(upperStr);

    char* id = malloc(maxDigits);

    long unsigned int invalidSum = 0;
    for (long unsigned int i = lower; i <= upper; ++i)
    {
        sprintf(id, "%ld", i);
        if (isValidId(id) == FALSE)
        {
            invalidSum += i;
            printf("Invalid id %s -> %ld: %ld\n", id, i, invalidSum);
        }
    }

    free(id);

    return invalidSum;
}

int mainReal(int argc, char** argv)
{
    // NEXT UP: The sum for part 2 is incorrect. Debug.
    if (argc < 2)
    {
        printf("Please supply input args\n");
        return -1;
    }

    const char* fileName = argv[1];
    FILE* inFile = fopen(fileName, "r");
    if (inFile == NULL)
    {
        printf("Error reading %s", fileName);
        fclose(inFile);
    }
    
    char inLine[1000];

    if (fgets(inLine, sizeof(inLine), inFile) == NULL)
    {
        printf("Error reading file contents");
        return -1;
    }

    long unsigned int totalInvalid = 0;

    char* rangeStart = inLine;
    char* rangeEnd = rangeStart;

    while(*rangeEnd != '\0')
    {
        if (*rangeEnd != ',' && *rangeEnd != '\n')
        {
            ++rangeEnd;
            continue;
        }
        // printf("%s", rangeStart);

        // This is some gross string manipulation hack
        *rangeEnd = '\0';
        ++rangeEnd; // already pointing to next range

        long unsigned int newSum = invalidIdSum(rangeStart);
        printf("%lu + %lu\n", totalInvalid, newSum);
        if ((LONG_MAX - totalInvalid) < newSum)
        {
            printf("Hit overflow");
            return 1;
        }

        totalInvalid += newSum;
        printf("Invalid sum in range is %lu\n", totalInvalid);
        rangeStart = rangeEnd;
    }
    printf("LONG_MAX: %lu\n", LONG_MAX);

    fclose(inFile);

    return 0;
}

int mainDebug(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        printf("%s is %s\n", argv[i], isValidId(argv[i]) == TRUE ? "valid" : "invalid");

    return 0;
}

int main(int argc, char** argv)
{
    // int res = mainDebug(argc, argv);
    int res = mainReal(argc, argv);

    return res;
}
