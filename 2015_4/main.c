#include <stdio.h>
#include <stdlib.h>

#include <openssl/md5.h>

// Compiler command
// cl main.c /I C:\Users\pedro.sanchez\.conan\data\OpenSSL\3.1.4\_\_\package\164640aad040835ac89882393a96d89200694f04\include /L C:\Users\pedro.sanchez\.conan\data\OpenSSL\3.1.4\_\_\package\164640aad040835ac89882393a96d89200694f04\lib\libcrypto.lib

char* extractHash(char* InputString)
{
    while (*InputString != ':')
    {
        ++InputString;
    }

    // Hardcoded to process the output of Get-FileHash
    ++InputString;
    ++InputString;

    return InputString;
}

void generateKeyFile(const char* secretKey, const char* fileName, int number)
{
    char fullKey[64];
    sprintf_s(fullKey, sizeof(fullKey), "%s%d", secretKey, number); // The Input 1 example
    // printf("%s\n", fullKey);

    // Write it to temp file
    FILE* tempFile = fopen(fileName, "w");
    fprintf(tempFile, "%s", fullKey);
    fclose(tempFile);
}

char* getFileHash(const char* hashFile, char* buffer)
{
    char command[256];
    // TODO: this is slow as hell. Use openssl/md5.h
    sprintf_s(command, sizeof(command), "powershell -command \"Get-FileHash %s -Algorithm MD5 | Format-List -Property Hash\"", hashFile); // The Input 1 example
    // printf("%s\n", command);
    FILE* commandStream = _popen(command, "r");
    if (commandStream == NULL)
        exit(1);

    // Extract the hash
    char* outputHash = NULL;
    while(fgets(buffer, 1024, commandStream) != NULL)
    {
        if (buffer[0] != 'H')
            continue;

        outputHash = extractHash(buffer);
    }

    _pclose(commandStream);
    return outputHash;
}

int isTargetHash(const unsigned char* hash)
{
    return !hash[0] && !hash[1] && !hash[2];
    // return !hash[0] && !hash[1] && hash[2] < 0x10; // Part 1 solution
}

int hashForNumber(int number, const char* key)
{
    #if 0
    const char* tempFileName = "key.tmp";
    generateKeyFile(key, tempFileName, number);

    // Run Get-FileHash
    char hashBuffer[1024];
    hash = getFileHash(tempFileName, hashBuffer);
    #endif

    unsigned char input[32];
    // sprintf_s(input, sizeof(input), "%s%d", key, number);
    unsigned char hash[16];
    // This is outputing gibberish, but getting somewhere
    int len = snprintf(input, sizeof(input), "%s%ld", key, number);
    MD5(input, len, hash);
    // printf("Input %s\n", input);

    // Check it starts with 5 zeroes
    // NEXT: Need to change the way we compare the hash for that to be hex
    int result = isTargetHash(hash);
    if (result == 1)
    {
        printf("Hash 0x");
        for (int i = 0; i < 16; ++i)
            printf("%02x", hash[i]);

        printf(" is the target\n");
    }
    return result;
}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        printf("Input required");
        return -1;
    }

    // int number = 0;
    int number = 282749; // This is the number we hit for first solution
    int printCounter = 0;
    while(hashForNumber(number, argv[1]) == 0)
    {
        ++number;
        if (printCounter++ % 10000 == 0)
        {
            printf("Tried number %d\n", number);
            printCounter = 0;
        }
            
    }
    printf("Number is %d\n", number);
}
