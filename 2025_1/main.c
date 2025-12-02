#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIAL_MAX 100 // Maximum number is 99

int nextNumber(int in, char* inst, int* clicksOnZero)
{
    // Parse string
    char* numberStr = inst + 1;
    int delta = atoi(numberStr);
    int mult = 1;
    if (*inst == 'L')
    {
        mult = -1;
    }
    else if (*inst != 'R')
    {
        printf("Wrong read\n");
    }
    printf("Delta %d\n", mult * delta);

    // Next number
    int out = in + mult * delta;
    printf("Bare out %d\n", out);

    // Part 2. I couldn't be bothered to recreate the whole math, so just went with a 1 by 1 counter.
    int secondOut = in;
    for (int i = delta; i > 0; --i)
    {
        secondOut += mult;

        if (secondOut == DIAL_MAX)
        {
            secondOut = 0;
        }
        else if (secondOut < 0)
        {
            secondOut = 99;
        }

        if (secondOut == 0)
        {
            ++(*clicksOnZero);
        }
    }
    printf("SecondOut %d, clicksOnZero %d\n", secondOut, *clicksOnZero);

    // Part 1
    while (out < 0 || out >= DIAL_MAX)
    {
        if (out < 0)
        {
            out = DIAL_MAX + out; // out is negative here
        }
        else if (out >= DIAL_MAX)
        {
            out = out - DIAL_MAX;
        }
    }

    return out;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Missing arguments\n");
        return 1;
    }


    const char* fileName = argv[1];
    FILE* inFile = fopen(fileName, "r");

    if (inFile == NULL)
    {
        printf("Error reading %s", fileName);
        fclose(inFile);
    }

    char line[10];
    int dial = 50;
    int password = 0;
    int clicksOnZero = 0; // Part 2
    while (fgets(line, sizeof(line), inFile) != NULL)
    {
        printf("%s", line);
        int prevDial = dial;
        dial = nextNumber(dial, line, &clicksOnZero);
        printf("%d -> %d\n\n", prevDial, dial);

        if (dial == 0)
        {
            ++password;
        }
    }
    printf("\n");

    printf("Password is %d\n", password);
    printf("Password part 2 is %d\n", clicksOnZero);

    fclose(inFile);
}
