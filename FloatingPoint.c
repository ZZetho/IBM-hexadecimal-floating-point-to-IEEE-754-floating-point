#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    uint8_t sign : 1;
    uint8_t exponent : 7;
    uint32_t fraction : 24;
}
IBMSingle;

typedef struct
{
    uint8_t sign : 1;
    uint8_t exponent : 7;
    uint64_t fraction : 56;
}
IBMDouble;

typedef struct
{
    uint8_t sign : 1;
    uint8_t exponent : 8;
    uint32_t fraction : 23;
}
IEEESingle;

typedef struct
{
    uint8_t sign : 1;
    uint16_t exponent : 11;
    uint64_t fraction : 52;
}
IEEEDouble;

int main(int argc, char * argv[])
{
    FILE * inputFile;
    FILE * outputFile;

    char inputFilename[1024];
    char outputFilename[1024];

    char inputPrecisionChar;
    char outputPrecisionChar;

    uint8_t inputPrecision;
    uint8_t outputPrecision;

    const uint8_t SINGLE = 1;
    const uint8_t DOUBLE = 2;

    // get the input filename from the user and its precision
    printf("Enter input filename: ");
    if (fscanf(stdin, " %1023[^\n]", inputFilename) != 1)
    {
        printf("No file name given\n");
        return 0;
    }
    // get the precision of the input file from the user
    printf("Enter input precision, (S for single, D for double): ");
    if (fscanf(stdin, " %1[^\n]", &inputPrecisionChar) != 1)
    {
        printf("No precision given\n");
        return 0;
    }
    if (inputPrecisionChar == 'D')
    {
        inputPrecision = DOUBLE;
    }
    else if (inputPrecisionChar == 'S')
    {
        inputPrecision = SINGLE;
    }
    else
    {
        printf("Unknown precision: %c\n", inputPrecisionChar);
        return 0;
    }

    // open the input file
    if ((inputFile = fopen(inputFilename, "rb")) == NULL)
    {
        printf("Failed to open: %s\n", inputFilename);
        return 1;
    }
    //printf("opened: %s\n", inputFilename);

    // get the output filename from the user
    printf("Enter output filename: ");
    if (fscanf(stdin, " %1023[^\n]", outputFilename) != 1)
    {
        printf("No file name given\n");
        return 0;
    }
    // get the precision of the output file from the user
    printf("Enter output precision, (S for single, D for double): ");
    if (fscanf(stdin, " %1[^\n]", &outputPrecisionChar) != 1)
    {
        printf("No precision given\n");
        return 0;
    }
    if (outputPrecisionChar == 'D')
    {
        outputPrecision = DOUBLE;
    }
    else if (outputPrecisionChar == 'S')
    {
        outputPrecision = SINGLE;
    }
    else
    {
        printf("Unknown precision: %c\n", inputPrecisionChar);
        return 0;
    }

    // open the output file
    if ((outputFile = fopen(outputFilename, "wb")) == NULL)
    {
        printf("Failed to open: %s\n", outputFilename);
        return 1;
    }
    //printf("opened: %s\n", outputFilename);

    if (inputPrecision == DOUBLE && outputPrecision == DOUBLE)
    {

    }
    else if (inputPrecision == DOUBLE && outputPrecision == SINGLE)
    {
        // input double
        // output single
    }
    else if (inputPrecision == SINGLE && outputPrecision == DOUBLE)
    {
        // input single
        // output single
    }
    else if (inputPrecision == SINGLE && outputPrecision == SINGLE)
    {
        uint8_t inputBuffer[4];
        uint8_t outputBuffer[4];

        // read each line into the input buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 4, 1, inputFile) == 1)
        {
            // input buffer stores bytes in reverse order

            //IBMSingle input;
            //input.sign = ((inputBuffer & 0x80) >> 7);

            //printf("%x", input.sign);

            // write to the output file
            fwrite(outputBuffer, sizeof(uint8_t) * 4, 1, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
