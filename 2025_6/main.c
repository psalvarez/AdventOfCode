#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef struct Operations
{
    size_t** operands; // rows->columns
    char* operators;
} Operations;

// I'm using dynamically allocated arrays for fun here
int main()
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

}
