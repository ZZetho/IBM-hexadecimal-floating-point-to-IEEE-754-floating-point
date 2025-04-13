#include <stdio.h>
#include <stdint.h>

int main(int argc, char * argv[])
{
    char * inputFilename = "input.txt";
    char * outputFilename = "output.txt";

    FILE * inputFile = fopen(inputFilename, "rb");
    FILE * outputFile = fopen(outputFilename, "wb");

    char inputPrecisionChar;
    char outputPrecisionChar;

    const uint8_t SINGLE = 1;
    const uint8_t DOUBLE = 2;

    uint8_t inputPrecision = SINGLE;
    uint8_t outputPrecision = SINGLE;

    /*
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
     */

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

        // read four bytes into the buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 4, 1, inputFile) == 1)
        {
            // input buffer stores bytes in reverse order

            //IBMSingle input;
            //input.sign = ((inputBuffer & 0x80) >> 7);

            uint8_t sign = (inputBuffer[0] & 0x80) >> 7;
            uint8_t exponent = (inputBuffer[0] & 0x7F);
            uint32_t fraction = (inputBuffer[1] << 16) | (inputBuffer[2] << 8) | inputBuffer[3];

            //printf("fraction: %x\n", fraction);
            //printf("exponent: %d\n", exponent);
            //printf("sign: %x\n", sign);

            // the number of bits that the fraction will be moved left by; ie: the index of the floating point
            uint16_t pointIndex = (exponent - 64) * 4;

            // find the index of the first 1 in the fraction
            int oneIndex;
            for (oneIndex = 0; oneIndex < 33; oneIndex ++)
            {
                if (oneIndex == 32)
                {
                    // TODO no ones found, = 0
                    break;
                }
                if (fraction >> oneIndex == 1)
                {
                    oneIndex = (32 - oneIndex) - 8;
                    break;
                }
            }

            // move the fraction left by oneIndex for the implicit 1 of IEEE
            fraction = (fraction << oneIndex) & 0xFFFFFF;

            // new exponent, IEEE is biased by 127 bits
            pointIndex = pointIndex - oneIndex + 127;

            //printf("%d\n", pointIndex);
            //printf("%x\n", fraction);

            uint32_t output = 0;
            output = output | ((uint32_t)sign << 31);
            output = output | ((uint32_t)pointIndex << 23);
            output = output | fraction >> 1;

            //printf("output: %x\n", output);

            // write the bytes of output to the output file in the correct order
            for (int byteIndex = sizeof(output) - 1; byteIndex >= 0; byteIndex --)
            {
                printf("e\n");
                // i <3 pointers
                fwrite(&((uint8_t*)&output)[byteIndex], 1, 1, outputFile);
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
