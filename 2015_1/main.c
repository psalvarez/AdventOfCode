#include <stdio.h>

#define MAX_STRING_SIZE 1000

int processInstructions(const char* fileName)
{
    FILE* fileHandle = fopen(fileName, "r");
    char newFloor[2]; // Need space for the null terminator
    int floorNumber = 0;
    int firstBasementChar = -1;
    int charCount = 0;
    while (fgets(newFloor, 2, fileHandle) != NULL)
    {
        printf("%s", newFloor);
        if (newFloor[0] == ')')
        {
            --floorNumber;
        }
        else if (newFloor[0] == '(')
        {
            ++floorNumber;
        }
        ++charCount;
        if ((firstBasementChar == -1) && (floorNumber == -1)) // If we aren't in the basement but we haven't been here before...
            firstBasementChar = charCount;
        // printf("%d", floorNumber);
    }
    fclose(fileHandle);
    printf("\n");
    printf("Basement char: %d\n", firstBasementChar);
    return floorNumber;
}

int main()
{
    const char FILE_NAME[] = "InputFile.txt";
    // const char FILE_NAME[] = "TestInput1.txt";
    int FinalFloor = processInstructions(FILE_NAME);
    printf("Santa ends in floor %d\n", FinalFloor);

    return 0;
}