#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define N_MAX_HOUSES 10000

struct HouseCoords
{
    int x;
    int y;
};

struct HouseList
{
    struct HouseCoords coordList[N_MAX_HOUSES];
    int count;
};


int areTheSameHouse(struct HouseCoords* houseA, struct HouseCoords* houseB)
{
    return (houseA->x == houseB->x) && (houseA->y == houseB->y);
}

int HouseList_inList(struct HouseCoords* houseToCheck, struct HouseList* houses)
{
    if (!houses || !houseToCheck || !houses->coordList)
        return FALSE;

    // Since we are moving only to contiguous cells in the grid, we are more likely to hit recently
    // visited houses than earlier ones
    for (int i = N_MAX_HOUSES - 1; i >= 0; --i)
    {
        struct HouseCoords someHouse = houses->coordList[i];
        if (areTheSameHouse(houseToCheck, &someHouse))
            return TRUE;
    }

    return FALSE;
}

void HouseList_add(struct HouseCoords* newHouse, struct HouseList* houses)
{
    // printf("%d %d %d", houses->count, newHouse->x, newHouse->y);
    // Line below seems to be somehow crashing
    houses->coordList[houses->count] = *newHouse;
    ++houses->count;
}

void addHouseToList(struct HouseCoords* newHouse, struct HouseList* houses)
{
    // printf("Adding %d %d", newHouse->x, newHouse->y);
    if (!HouseList_inList(newHouse, houses))
    {
        // printf(" - not in list, adding\n");
        HouseList_add(newHouse, houses);
    }
}

void processNewDirection(char newDirection, struct HouseCoords* currentHouse, struct HouseList* visitedHouses)
{
    // printf("%s ", newDirection);
    if (newDirection == '>')
        ++currentHouse->x;
    else if (newDirection == '<')
        --currentHouse->x;
    if (newDirection == '^')
        ++currentHouse->y;
    if (newDirection == 'v')
        --currentHouse->y;

    addHouseToList(currentHouse, visitedHouses);

}

void processInput(const char* inputFile)
{
    struct HouseList visitedHouses= {.count = 1};
    struct HouseCoords santaCurrentHouse;
    santaCurrentHouse.x = 0;
    santaCurrentHouse.y = 0;
    struct HouseCoords robotCurrentHouse;
    robotCurrentHouse.x = 0;
    robotCurrentHouse.y = 0;

    // For every new input
    char newDirection[2];
    FILE* directionsHandle = fopen(inputFile, "r");
    int santasTurn = 0;
    while (fgets(newDirection, 2, directionsHandle))
    {
        // call processNewDirection, once for santa, another for robot, always passing the same visitedHouses list.
        processNewDirection(newDirection[0], santasTurn ? &santaCurrentHouse : &robotCurrentHouse, &visitedHouses);
        if (++santasTurn > 1)
            santasTurn = 0;
    }
    printf("Visited a total of %d houses", visitedHouses.count);
    fclose(directionsHandle);
}

int main (int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Error: input file required\n");
        printf("Usage:\n");
        printf("./main.exe <input_file_path>\n");
        return -1;
    }

    const char* fileName = argv[1];
    processInput(fileName);
}