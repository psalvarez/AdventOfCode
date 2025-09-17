#include <stdio.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

enum boool isVowel(char c) {
    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return 1;
        default:
            return 0;
    }
}

enum bool
{
    FALSE,
    TRUE
};

enum bool isNaughtySubstring(const char first, const char second) 
{
    if ((first == 'a') && (second == 'b') ||
        (first == 'c') && (second == 'd') ||
        (first == 'p') && (second == 'q') ||
        (first == 'x') && (second == 'y'))
    {
        // printf("Naughty first char: %c\n", input);
        return TRUE;
    }
    
    return FALSE;
}

int isStringNice1(const char* input)
{
    int vowelCount = 0;
    enum bool twoCharFound = FALSE;
    char previous = '\0';
    for (int i = 0; input[i] != '\0'; ++i)
    {
        vowelCount += isVowel(input[i]);
        if (twoCharFound == FALSE)
        {
            if (previous == input[i])
            {
                twoCharFound = TRUE;
            }
        }
        
        if (isNaughtySubstring(previous, input[i]))
        {
            printf("Naughty pair %c%c found on %s", previous, input[i], input);
            return 1;
        }
        previous = input[i];
    }

    if (vowelCount >= 3 && twoCharFound == TRUE)
        return 0;

    printf("%s, %s on %s\n", vowelCount < 3 ? "<3 vowels" : "", twoCharFound == FALSE ? "no two chars in a row" : "", input);
    return 1;
}

int isStringNice2(const char* input)
{
    // ------- Repeated, non-overlapping pair

    // 1. Iterate through string one by one
    const char* currentChar = input;
    enum bool hasRepeatedPair = FALSE;
    enum bool hasCharSandwich = FALSE;
    
    // No point in checking when we reach the penultimate char
    while (*(currentChar+2) != '\0')
    {
        // Repeated characters in a row can cause overlap, skip to two ahead
        const char* aheadChar = currentChar + 2;
        while(*(aheadChar+1) != '\0')
        {
            // printf("%c%c ", *currentChar, *aheadChar);
            if (*currentChar == *aheadChar)
            {
                if (hasRepeatedPair == FALSE && *(currentChar+1) == *(aheadChar+1))
                {
                    hasRepeatedPair = TRUE;
                    break;
                }

                // This is only useful in the first iteration really, but whatever
                if (hasCharSandwich == FALSE && (aheadChar - currentChar) == 2)
                {
                    hasCharSandwich = TRUE;
                }
            }

            if (hasCharSandwich == TRUE && hasRepeatedPair == TRUE)
                break;

            ++aheadChar;
        }

        if (hasCharSandwich == TRUE && hasRepeatedPair == TRUE)
            break;

        ++currentChar;
    }

    // 2. Find char in the substring of _next+1_->end letters (ensures no overlap)
    // 2.1. If it is there, check letter after for each of those
    // 2.2. If next letter is the same, repeated pair true, false if not
    // 2.3. Compare first repeated letter position. If difference is +2, second condition is met

    return hasRepeatedPair == TRUE && hasCharSandwich == TRUE ? 0 : 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    int niceCount = 0;
    int result = -1;
    while (input != NULL && !feof(input))
    {
        char line[256];
        fgets(line, sizeof(line), input);
        printf("\nLine %s", line);
        result = isStringNice2(line);
        printf(": %s\n", result == 0 ? "nice" : "naughty");
        if (result == 0) {
            ++niceCount;
        }
        // printf("%s is %s\n", line, result == 0 ? "nice" : "naughty");
    }
    printf("\n%d nice strings found\n", niceCount);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
