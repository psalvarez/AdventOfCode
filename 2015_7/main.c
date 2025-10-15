#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#pragma warning(disable : 5045) // Disable QSpectre mitigation warning
#endif

#define CIRCUIT_SIZE 1000

typedef struct Connection
{
    char inWire[2][10];
    char outWire[10];
    char command[10];
    int outValue; // Cache the output value once computed
} Connection;


// It will return -1 if gone through entire array and doesn't find it, but if wireOut is not in the
// array, behavior is undefined.
int findConnection(const char* wireOut, Connection circuit[CIRCUIT_SIZE])
{
    int ConnIdx = -1;

    for (int i = 0; i < CIRCUIT_SIZE; ++i)
    {
        if (strcmp(circuit[i].outWire, wireOut) == 0)
        {
            // printf("wireOut: %s; circuits Wire: %s; ConnIdx: %hu\n", wireOut, circuit[i].outWire, i);
            ConnIdx = i;
            break;
        }
    }

    // Perhaps return pointer, pair or flag in the struct in case it didn't find it?
    return ConnIdx;
}

int getWireOutValue(const char* wireOut, Connection circuit[CIRCUIT_SIZE]);

int computeValue(Connection* conn, Connection circuit[CIRCUIT_SIZE])
{
    printf("Computing command: %s, in1: %s, in2: %s, out: %s\n", conn->command, conn->inWire[0], conn->inWire[1], conn->outWire);
    if (conn->outValue != -1)
    {
        printf("Using cached value for %s: %hu\n", conn->outWire, conn->outValue);
        return conn->outValue;
    }

    int outValue = 0;
    if (strcmp(conn->command, "LSHIFT") == 0)
    {
        int inValue = getWireOutValue(conn->inWire[0], circuit);
        
        outValue = inValue << atoi(conn->inWire[1]);
    }
    else if (strcmp(conn->command, "RSHIFT") == 0)
    {
        int inValue = getWireOutValue(conn->inWire[0], circuit);
        outValue = inValue >> atoi(conn->inWire[1]);
    }
    else if (strcmp(conn->command, "AND") == 0)
    {
        int in1 = getWireOutValue(conn->inWire[0], circuit);
        int in2 = getWireOutValue(conn->inWire[1], circuit);
        outValue = in1 & in2;
    }
    else if (strcmp(conn->command, "OR") == 0)
    {
        int in1 = getWireOutValue(conn->inWire[0], circuit);
        int in2 = getWireOutValue(conn->inWire[1], circuit);
        outValue = in1 | in2;
    }
    else if (strcmp(conn->command, "NOT") == 0)
    {
        int inValue = getWireOutValue(conn->inWire[0], circuit);
        // printf("Calculating NOT %s (%hu);\n", conn->inWire[0], inValue);
        outValue = ~inValue;
    }
    else
    {
        // Check if it's a digit and convert
        if (conn->inWire[0][0] >= '0' && conn->inWire[0][0] <= '9')
            outValue = atoi(conn->inWire[0]);
        else
            outValue = getWireOutValue(conn->inWire[0], circuit);
    }

    conn->outValue = outValue;
    return outValue;
}

int getWireOutValue(const char* wireOut, Connection circuit[CIRCUIT_SIZE])
{
    // printf("Getting out value for %s; ", wireOut);
    int outValue = -1;
    if (wireOut[0] >= '0' && wireOut[0] <= '9')
    {
        return atoi(wireOut);
    }

    int outConnIdx = findConnection(wireOut, circuit);
    
    if (outConnIdx == -1)
    {
        fprintf(stderr, "Error: Wire '%s' not found in circuit\n", wireOut);
        exit(1);
    }

    if (outConnIdx != -1)
    {
        // printf("Computing...\n");
        outValue = computeValue(&circuit[outConnIdx], circuit);
    }
    else
    {
        printf("outConnIdx: %hu not found\n", outConnIdx);
    }
    
    // printf("wire %s: %hu\n", wireOut, outValue);
    return outValue;
}

void printCircuit(Connection circuit[CIRCUIT_SIZE])
{
    for (int i = 0; i < 8; ++i)
    {
        printf("Circuit[%hu] -> %s\n", i, circuit[i].outWire);
    }
}

void clearCircuitCache(Connection circuit[CIRCUIT_SIZE])
{
    for (int i = 0; i < CIRCUIT_SIZE; ++i)
    {
        circuit[i].outValue = -1;
    }
}

int main(int argc, const char** argv)
{
    if (argc < 3)
    {
        printf("Missing arguments\n");
        return 1;
    }

    // Read file
    const char* fileName = argv[1];
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error reading %s", fileName);
        fclose(file);
    }

    char line[40];
    Connection circuit[CIRCUIT_SIZE];
    int idx = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // printf("%s", line);
        char in1[20];
        char in2[20];
        char out[20];
        char command[10];
        if (2 <= sscanf(line, "%s -> %s", in1, out))
        {
            strcpy(circuit[idx].outWire, out);
            strcpy(circuit[idx].inWire[0], in1);
            // printf("Read value: %s\n", in1);
        }
        else if(4 <= sscanf(line, "%s %s %s -> %s", in1, command, in2, out))
        {
            strcpy(circuit[idx].inWire[0], in1);
            strcpy(circuit[idx].inWire[1], in2);
            strcpy(circuit[idx].command, command);
            strcpy(circuit[idx].outWire, out);
            // printf("Read operation: %s\n", command);
        }
        else if(2 <= sscanf(line, "NOT %s -> %s", in1, out))
        {
            strcpy(circuit[idx].inWire[0], in1);
            strcpy(circuit[idx].outWire, out);
            strcpy(circuit[idx].command, "NOT");
            // printf("Read NOT: %s, idx %hu\n", in1, idx);
        }
        circuit[idx].outValue = -1;
        // printf("inValue: %hu\n", circuit[idx].inValue);
        // printf("out: %s\n", out);
        // printf("\n");
        ++idx;
    }

    fclose(file);

    // printCircuit(circuit);

    // const char* wireOut[] = {"a"};// {"d", "e", "f", "g", "h", "i", "x", "y"};
    const char* wireOut = argv[2];
    int outValue = getWireOutValue(wireOut, circuit);
    printf("Wire %s has a value of %hu\n\n", wireOut, outValue);

    // PART 2
    // TODO: Find b's connection in the circuit, transform a's value into char and pass it to b
    printf("\n\n>>>>>> PART 2 <<<<<<<<\n");
    Connection* bConn;
    for (int i = 0; i < CIRCUIT_SIZE; ++i)
    {
        if (strcmp(circuit[i].outWire, "b") == 0)
        {
            bConn = &circuit[i];
            break;
        }
    }

    clearCircuitCache(circuit);
    char outValueStr[20];
    sprintf(outValueStr, "%d", outValue);
    strcpy(bConn->inWire[0], outValueStr);

    int outValue2 = getWireOutValue(wireOut, circuit);
    printf("Wire %s has a NEW value of %hu\n\n", wireOut, outValue2);

    return 0;
}
