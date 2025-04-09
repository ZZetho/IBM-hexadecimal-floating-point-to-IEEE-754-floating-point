#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char * argv[])
{
    FILE * input;
    FILE * output;

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
    if ((input = fopen(inputFilename, "r")) == NULL)
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
    if ((output = fopen(outputFilename, "w")) == NULL)
    {
        printf("Failed to open: %s\n", outputFilename);
        return 1;
    }
    //printf("opened: %s\n", outputFilename);

    char inputBuffer[1024];
    char outputBuffer[1024];

    // read each line into the input buffer
    while (fgets(inputBuffer, 1024, input) != NULL)
    {
        // TODO: convert from IBM to IEEE

        if (inputPrecision == DOUBLE && outputPrecision == DOUBLE)
        {
            // input double
            // output double
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
            // input single
            // output single
        }

        // write to the output file
        fprintf(output ,"%s", outputBuffer);
    }

    fclose(input);
    fclose(output);

    return 0;
}
