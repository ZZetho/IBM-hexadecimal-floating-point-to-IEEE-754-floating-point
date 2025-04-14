#include <stdio.h>
#include <stdint.h>

//void printIEEEsingle(uint32_t ieeeSingle)
//{
//    float * out = &ieeeSingle;
//
//    printf("%.8f\n", *out);
//}
//
//void printIEEEdouble(uint64_t ieeeDouble)
//{
//    double * out = &ieeeDouble;
//
//    printf("%.16lf\n", *out);
//}

int main(int argc, char * argv[])
{
    char inputFilename[1024];
    char outputFilename[1024];

    FILE * inputFile;
    FILE * outputFile;

    char inputPrecisionChar;
    char outputPrecisionChar;

    const uint8_t SINGLE = 1;
    const uint8_t DOUBLE = 2;

    uint8_t inputPrecision;
    uint8_t outputPrecision;


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
        // double to double
        uint8_t inputBuffer[8];
        // read eight bytes into the buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 8, 1, inputFile) == 1)
        {
            uint8_t sign = (inputBuffer[0] & 0x80) >> 7;
            uint16_t IBMexponent = (inputBuffer[0] & 0x7F);
            uint64_t fraction = ((uint64_t)inputBuffer[1] << 48) | ((uint64_t)inputBuffer[2] << 40) | (uint64_t)inputBuffer[3] << 32 | (uint64_t)inputBuffer[4] << 24 | (uint64_t)inputBuffer[5] << 16 | (uint64_t)inputBuffer[6] << 8 | (uint64_t)inputBuffer[7];

            //printf("fraction: %llx\n", fraction);
            //printf("exponent: %d\n", IBMexponent);
            //printf("sign: %x\n", sign);

            // find the index of the first 1 in the fraction
            uint16_t oneIndex;
            for (oneIndex = 0; oneIndex < 65; oneIndex++) {
                if (oneIndex == 64)
                {
                    // TODO no ones found, = 0
                    printf("todo\n");
                    break;
                }
                if (fraction >> oneIndex == 1)
                {
                    oneIndex = (64 - oneIndex) - 8;
                    break;
                }
            }
            //printf("%d\n", oneIndex);

            // move the fraction left by oneIndex for the implicit 1 of IEEE and cut off the first one
            fraction = (fraction << oneIndex) & 0xFFFFFFFFFFFFFF;

            // new exponent (number of bits the fraction should be moved)
            // the number of bits IBM moved it
            // minus the index of the first one as IEEE has an implicit one
            // IEEE double is biased by 1023
            uint16_t IEEEexponent = ((IBMexponent - 64) * 4) - oneIndex + 1023;

            uint64_t output = 0;
            output = output | ((uint64_t) sign << 63);
            output = output | ((uint64_t) IEEEexponent << 52);
            output = output | fraction >> 4;

            //printf("%llx\n", output);

            //printIEEEdouble(output);

            // write the bytes of output to the output file in the correct order
            for (int byteIndex = sizeof(output) - 1; byteIndex >= 0; byteIndex--)
            {
                // i <3 pointers
                fwrite(&((uint8_t *) &output)[byteIndex], 1, 1, outputFile);
            }
        }
    }
    else if (inputPrecision == DOUBLE && outputPrecision == SINGLE)
    {
        // double to single
        uint8_t inputBuffer[8];
        // read eight bytes into the buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 8, 1, inputFile) == 1)
        {
            uint8_t sign = (inputBuffer[0] & 0x80) >> 7;
            uint16_t IBMexponent = (inputBuffer[0] & 0x7F);
            uint64_t fraction = ((uint64_t)inputBuffer[1] << 48) | ((uint64_t)inputBuffer[2] << 40) | (uint64_t)inputBuffer[3] << 32 | (uint64_t)inputBuffer[4] << 24 | (uint64_t)inputBuffer[5] << 16 | (uint64_t)inputBuffer[6] << 8 | (uint64_t)inputBuffer[7];

            printf("fraction: %llx\n", fraction);
            printf("exponent: %d\n", IBMexponent);
            printf("sign: %x\n", sign);

            // find the index of the first 1 in the fraction
            uint16_t oneIndex;
            for (oneIndex = 0; oneIndex < 65; oneIndex++) {
                if (oneIndex == 64)
                {
                    // TODO no ones found, = 0
                    printf("todo\n");
                    break;
                }
                if (fraction >> oneIndex == 1)
                {
                    oneIndex = (64 - oneIndex) - 8;
                    break;
                }
            }
            //printf("%d\n", oneIndex);

            // move the fraction left by oneIndex for the implicit 1 of IEEE and cut off the first one
            fraction = (fraction << oneIndex) & 0xFFFFFFFFFFFFFF;

            // new exponent (number of bits the fraction should be moved)
            // the number of bits IBM moved it
            // minus the index of the first one as IEEE has an implicit one
            // IEEE single is biased by 127
            uint16_t IEEEexponent = ((IBMexponent - 64) * 4) - oneIndex + 127;

            uint32_t output = 0;
            output = output | ((uint32_t) sign << 31);
            output = output | ((uint32_t) IEEEexponent << 23);
            output = output | (uint32_t)(fraction >> 33);

            //printf("%llx\n", output);

            //printIEEEsingle(output);

            // write the bytes of output to the output file in the correct order
            for (int byteIndex = sizeof(output) - 1; byteIndex >= 0; byteIndex--)
            {
                // i <3 pointers
                fwrite(&((uint8_t *) &output)[byteIndex], 1, 1, outputFile);
            }
        }
    }
    else if (inputPrecision == SINGLE && outputPrecision == DOUBLE)
    {
        // single to double
        uint8_t inputBuffer[4];
        // read four bytes into the buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 4, 1, inputFile) == 1)
        {
            uint8_t sign = (inputBuffer[0] & 0x80) >> 7;
            uint8_t IBMexponent = (inputBuffer[0] & 0x7F);
            uint32_t fraction = (inputBuffer[1] << 16) | (inputBuffer[2] << 8) | inputBuffer[3];

            //printf("fraction: %x\n", fraction);
            //printf("exponent: %d\n", exponent);
            //printf("sign: %x\n", sign);

            // find the index of the first 1 in the fraction
            uint16_t oneIndex;
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

            // new exponent (number of bits the fraction should be moved)
            // the number of bits IBM would move the fraction
            // minus the index of the first 1, which moves the fraction left and cuts off the first one, for IEEE's implicit one
            // IEEE double is biased by 1023
            uint16_t IEEEexponent = ((IBMexponent - 64) * 4) - oneIndex + 1023;

            uint64_t output = 0;
            output = output | ((uint64_t)sign << 63);
            output = output | ((uint64_t)IEEEexponent << 52);
            output = output | (uint64_t)fraction << 28;

            //printf("output: %x\n", output);

            //printIEEEdouble(output);

            // write the bytes of output to the output file in the correct order
            for (int byteIndex = sizeof(output) - 1; byteIndex >= 0; byteIndex --)
            {
                // i <3 pointers
                fwrite(&((uint8_t*)&output)[byteIndex], 1, 1, outputFile);
            }
        }
    }
    else if (inputPrecision == SINGLE && outputPrecision == SINGLE)
    {
        // single to single
        uint8_t inputBuffer[4];
        // read four bytes into the buffer
        while (fread(inputBuffer, sizeof(uint8_t) * 4, 1, inputFile) == 1)
        {
            uint8_t sign = (inputBuffer[0] & 0x80) >> 7;
            uint8_t IBMexponent = (inputBuffer[0] & 0x7F);
            uint32_t fraction = (inputBuffer[1] << 16) | (inputBuffer[2] << 8) | inputBuffer[3];

            //printf("fraction: %x\n", fraction);
            //printf("exponent: %d\n", exponent);
            //printf("sign: %x\n", sign);

            // find the index of the first 1 in the fraction
            uint16_t oneIndex;
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

            // new exponent (number of bits the fraction should be moved)
            // the number of bits IBM would move the fraction
            // minus the index of the first 1, which moves the fraction left and cuts off the first one, for IEEE's implicit one
            // IEEE single is biased by 127
            uint16_t IEEEexponent = ((IBMexponent - 64) * 4) - oneIndex + 127;

            uint32_t output = 0;
            output = output | ((uint32_t)sign << 31);
            output = output | ((uint32_t)IEEEexponent << 23);
            output = output | fraction >> 1;

            //printf("output: %x\n", output);

            //printIEEEsingle(output);

            // write the bytes of output to the output file in the correct order
            for (int byteIndex = sizeof(output) - 1; byteIndex >= 0; byteIndex --)
            {
                // i <3 pointers
                fwrite(&((uint8_t*)&output)[byteIndex], 1, 1, outputFile);
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
