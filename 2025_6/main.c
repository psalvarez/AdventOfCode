#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include <stddef.h>

typedef struct Operations
{
    size_t** operands; // rows->columns
    char* operators;
} Operations;

size_t getOperand(int idx, size_t nLines, char** lines)
{
    size_t operand = 0;
    int nDigits = 0;
    for (int line = nLines - 2; line >= 0; --line) // Last line is the operator
    {
        char digit = lines[line][idx];
        if (digit == ' ')
            continue;
        size_t number = digit - '0';
        // We subtract 2 to account for i) the operand line, and ii) the array starting at idx == 0
        operand += number * pow(10, nDigits);
        ++nDigits;
        printf("%c - %zu - 10 ^ %d = %zu\n", digit, number, nDigits, operand);
    }

    return operand;
}

char getOperator(char readChar, char currentOp)
{
    if(readChar != ' ' && readChar != '\n')
        return readChar;

    return currentOp;
}

// I'm using dynamically allocated arrays for fun here
size_t partTwo()
{
    char* inBuffer = NULL;
    char** lines = NULL;
    size_t bufferSize = 0;
    size_t nLines = 0;
    ssize_t charsRead = 0;
    while((charsRead = getline(&inBuffer, &bufferSize, stdin)) != -1) // getline manages memory for us
    {
        lines = realloc(lines, (nLines + 1) * sizeof(char*));
        lines[nLines] = malloc(charsRead); // lines[i] is uninitialised at this point, so realloc would be undefined behavior.

        memcpy(lines[nLines], inBuffer, charsRead);

        printf("%zu,%zu - %s", nLines, charsRead, lines[nLines]);
        ++nLines;
    }

    int idx = 0;
    char operator = 'n';
    size_t total = 0;
    size_t opResult = 0;
    // while we don't find EOL, for each column
    while (lines[0][idx] != '\n')
    {
        printf("=========== %c --\n", lines[0][idx]);
        operator = getOperator(lines[nLines - 1][idx], operator);
        // Read all lines - 1 (i.e., all lines except operator) char by char
        size_t operand = getOperand(idx, nLines, lines);
        printf("%zu %c\n", operand, operator);

        if (operand == 0)
        {
            // Separator column reached, restart op result
            total += opResult;
            opResult = 0;
        }
        else
        {
            if (operator == '*')
            {
                opResult = opResult == 0 ? operand : operand * opResult;
            }
            else if (operator == '+')
            {
                opResult += operand;
            }
        }
        ++idx;
    }
    // Add result of last op
    total += opResult;
    printf("%zu\n", opResult);
    return total;
}

int partOne()
{
    char* inBuffer = NULL;

    char* token;
    char delimiters[] = {" \n"};
    size_t size = sizeof(char);
    Operations ops;
    size_t nOperands = 0;
    size_t nOperations = 0;
    ops.operands = malloc(0); // Allocate for the line of operands

    while(getline(&inBuffer, &size, stdin) != -1) // getline manages memory for us
    {
        printf("--- %zu ---\n", nOperands + 1);
        token = strtok(inBuffer, delimiters); 

        // Read operators line
        if (*token < '0' || *token > '9')
        {
            ops.operators = malloc(nOperations * sizeof(char));
            char* current = ops.operators;
            while (token != NULL)
            {
                printf("%s-\n", token);
                *current = *token;
                ++current;
                
                // read next token
                token = strtok(NULL, delimiters); 
            }
            break; // This is the last line, so don't continue
        }

        // Read operands
        ops.operands = realloc(ops.operands, (nOperands + 1) * sizeof(size_t*));
        ops.operands[nOperands] = malloc(0); // Allocate for the column
        nOperations = 0;
        while(token != NULL)
        {
            ops.operands[nOperands] = realloc(ops.operands[nOperands], (nOperations + 1) * sizeof(size_t));
            ops.operands[nOperands][nOperations] = atoi(token);
            printf("%s -> %zu-\n", token, ops.operands[nOperands][nOperations]);

            // read next token
            token = strtok(NULL, delimiters); 

            ++nOperations;
        }

        ++nOperands;
    }

    printf("---- %zu - %zu\n", nOperands, nOperations);
    size_t total = 0;
    for (size_t i = 0; i < nOperations; ++i)
    {
        size_t result = ops.operands[0][i];
        char operator = ops.operators[i];
        printf("%zu ", result);
        for (size_t j = 1; j < nOperands; ++j)
        {
            int operand = ops.operands[j][i];
            printf("%c %d ", operator, operand);
            if (operator == '+')
            {
                result += operand;
            }
            else if (ops.operators[i] == '*')
            {
                result *= operand;
            }
        }
        printf("= %zu\n", result);
        total += result;
    }

    // >920578270
    printf("%zu\n", total);

    return total;
}

int main()
{
    // printf("%d", partOne());
    size_t result = partTwo();
    printf("------\n");
    printf("%zu\n", result);
    return 0;
}
