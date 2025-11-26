// result for TestIn.txt: (2 + 5 + 10 + 6 = 23) - (0 + 3 + 7 + 1 = 11) = 12

// Will need to find a way to read _literally_ the characters on each line, and
// then parse them. Hopefully simply sprintf should work?
#include <stdio.h>
#include <stdlib.h>

typedef struct LineCount
{
    int input;
    int processed;
} LineCount;

void encodeLine(char* inLine, LineCount* count)
{
    // On each iteration of this loop we increase the *input* by one character
    while (*inLine != '\n')
    {
        printf("inLine %c, full %d, encoded %d\n", *inLine, count->input, count->processed);
        if (*inLine == '"')
        {
            ++inLine;
            ++count->input;
            count->processed += 2;
            continue;
        }
        else if (*inLine == '\\')
        {
            ++inLine; // Get the next char
            ++count->input;
            count->processed += 2; // Count the input backslash and the escaping one
        }
        else
        {
            // Count as one character to parse
            ++inLine;
            ++count->input;
            ++count->processed;
        }
    }

    count->processed += 2; // Surrounding double quotes
}

char* parseLine(char* inLine, LineCount* count)
{
    // We know it's going to be max the size of the inputLine
    char* outLine = malloc(sizeof(inLine));
    char* outChar = outLine;
    printf("Parsing %s", inLine);

    // On each iteration of this loop we increase the *output* by one character, except if it is a surrounding double quote
    while (*inLine != '\n')
    {
        printf("inLine %c, full %d\n", *inLine, count->input);
        if (*inLine == '"')
        {
            // Count it in the full and skip
            ++inLine;
            ++count->input;
            continue;
        }
        else if (*inLine == '\\')
        {
            ++inLine; // Get the next char
            ++count->input;

            *outChar = *inLine;

            // We have an x, so the next two only count as one
            if (*inLine == 'x')
            {
                inLine += 3;
                count->input += 3;
            }
            else
            {
                // We should just count it as the character it is and skip to the next
                ++inLine;
                ++count->input;
            }
        }
        else
        {
            // Count as one character to parse
            *outChar = *inLine;

            ++inLine;
            ++count->input;
        }
        ++outChar;
        ++count->processed;
    }

    printf("\n");

    *outChar = '\0'; // This is to avoid printing gargbage characters because we are over mallocing
    return outLine;
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

    char line[80];
    char* parsedLine;
    LineCount aggregate;
    aggregate.input = 0;
    aggregate.processed = 0;

    LineCount aggregate2; // For Part 2
    aggregate2.input = 0;
    aggregate2.processed = 0;
    while (fgets(line, sizeof(line), inFile) != NULL)
    {
        LineCount count;
        count.input = 0;
        count.processed = 0;
        printf("Read line: %s\n", line);
        parsedLine = parseLine(line, &count);
        printf("Parsed %s: %d -> %d\n", parsedLine, count.input, count.processed);
        
        aggregate.input += count.input;
        aggregate.processed += count.processed;
        free(parsedLine);

        encodeLine(line, &aggregate2);
        printf("Encoded: %d -> %d\n", aggregate2.input, aggregate2.processed);
    }

    printf("Final: %d - %d = %d\n", aggregate.input, aggregate.processed, aggregate.input - aggregate.processed);
    printf("Final Pt2: %d - %d = %d\n", aggregate2.processed, aggregate2.input, aggregate2.processed - aggregate2.input);
}
