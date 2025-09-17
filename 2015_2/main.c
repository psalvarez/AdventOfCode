#include <stdio.h>

#define N_DIFFERENT_SIDES 3

int calculateSideArea(int dimA, int dimB)
{
    return dimA * dimB;
}

int calculateBoxMeasurements(int* dimensions, int* outAreas, int* outPerimeters)
{
    int d = 0;
    int volume = 1;
    for (int i = 0; i < N_DIFFERENT_SIDES; ++i)
    {
        // Combine dimension i with all the ones that _haven't_ been combined yet
        for (int j = i + 1; j < N_DIFFERENT_SIDES; ++j)
        {
            outAreas[d] = dimensions[i] * dimensions[j];
            outPerimeters[d] = 2*dimensions[i] + 2*dimensions[j];
            ++d;
        }
        volume *= dimensions[i];
    }

    return volume;
}

struct BoxResources
{
    int* Dimensions;
    int PaperNeeded;
    int RibbonNeeded;
};

// Change this function to receive as an array
void resourcesNeededForBox(int* dimensionsArray, int* outPaperNeeded, int* outRibbonNeeded)
{
    int sideAreas[] = {-1, -1, -1};
    int perimeters[] = {-1, -1, -1};
    int volume = calculateBoxMeasurements(dimensionsArray, sideAreas, perimeters);

    int smallestSideArea = sideAreas[0];
    int smallestSidePerimeter = perimeters[0];
    int totalBoxSurface = 0;
    for (int i = 0; i < N_DIFFERENT_SIDES; ++i)
    {
        // printf("%d\n", sideAreas[i]);
        totalBoxSurface += 2 * sideAreas[i];
        if (sideAreas[i] < smallestSideArea)
        {
            smallestSideArea = sideAreas[i];
            smallestSidePerimeter = perimeters[i];
            // printf("%d\n", smallestSidePerimeter);
        }
    }
    
    *outPaperNeeded = totalBoxSurface + smallestSideArea;
    *outRibbonNeeded = smallestSidePerimeter + volume;
}

void parseDimensions(const char* dimensionsInput, int* dimensionsOutput)
{
    sscanf(dimensionsInput, "%dx%dx%d", &dimensionsOutput[0], &dimensionsOutput[1], &dimensionsOutput[2]);
}

void calculateTotalResources(const char* boxListPath, int* totalPaperNeeded, int* totalRibbonNeeded)
{
    FILE* boxListHandle = fopen(boxListPath, "r");
    char dimensionsLine[50];
    while (fgets(dimensionsLine, 20, boxListHandle))
    {
        // printf("%s\n", dimensionsLine);
        int dimensions[N_DIFFERENT_SIDES] = {-1, -1, -1};
        parseDimensions(dimensionsLine, dimensions);
        // Pass a struct with the dimensions and return ribbon and paper
        int paperForBox = 0;
        int ribbonForBox = 0;
        resourcesNeededForBox(dimensions, &paperForBox, &ribbonForBox);
        *totalPaperNeeded += paperForBox;
        *totalRibbonNeeded += ribbonForBox;
    }
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        printf("Error: input file required\n");
        printf("Usage:\n");
        printf("./main.exe <input_file_path>\n");
        return -1;
    }

    const char* fileName = argv[1];
    // Parse arguments
    int paper = 0;
    int ribbon = 0;
    calculateTotalResources(fileName, &paper, &ribbon);
    printf("Total paper Needed: %d\n", paper);
    printf("Total ribbon Needed: %d\n", ribbon);

    return 0;
}
