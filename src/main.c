#include "csv.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const char* inputFile = "input.csv";
    const char* outputFile = "output.txt";

    if (argc >= 2)
        inputFile = argv[1];
    if (argc >= 3)
        outputFile = argv[2];

    FILE* input = fopen(inputFile, "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file %s\n", inputFile);
        return 1;
    }

    Table* table = csvParser(input);
    fclose(input);

    FILE* output = fopen(outputFile, "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot create output file %s\n", outputFile);
        if (table)
            freeTable(table);
        return 1;
    }

    if (table != NULL) {
        tableMaker(output, table);
        freeTable(table);
    }

    fclose(output);
    return 0;
}
