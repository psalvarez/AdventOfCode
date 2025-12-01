#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIAL_MAX 100 // Maximum number is 99

int nextNumber(int in, char* inst)
{
    // Parse string
    char* numberStr = inst + 1;
    int delta = atoi(numberStr);
    printf("Delta %d\n", delta);
    int mult = 1;
    if (*inst == 'L')
    {
        mult = -1;
    }
    else if (*inst != 'R')
    {
        printf("Wrong read\n");
    }

    // Next number
    int out = in + mult * delta;
    printf("Bare out %d\n", out);
    
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
    while (fgets(line, sizeof(line), inFile) != NULL)
    {
        dial = nextNumber(dial, line);
        printf("%d -> %s\n", dial, line);

        if (dial == 0)
        {
            ++password;
        }
    }
    printf("\n");

    printf("Password is %d\n", password);

    fclose(inFile);
}
