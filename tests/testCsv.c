#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareFiles(const char* firstFilePath, const char* secondFilePath)
{
    FILE* firstFile = fopen(firstFilePath, "r");
    FILE* secondFile = fopen(secondFilePath, "r");

    if (!firstFile || !secondFile) {
        if (firstFile)
            fclose(firstFile);
        if (secondFile)
            fclose(secondFile);
        return 0;
    }

    int characterFromFirstFile;
    int characterFromSecondFile;

    while (1) {
        characterFromFirstFile = fgetc(firstFile);
        characterFromSecondFile = fgetc(secondFile);
        if (characterFromFirstFile != characterFromSecondFile) {
            fclose(firstFile);
            fclose(secondFile);
            return 0;
        }

        if (characterFromFirstFile == EOF) {
            break;
        }
    }
    fclose(firstFile);
    fclose(secondFile);
    return 1;
}

int test1()
{
    printf("\nTest 1: Empty file\n");
    int result = compareFiles("output1.txt", "expected1.txt");
    if (result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return result;
}

int test2()
{
    printf("\nTest 2: Really long text\n");
    int result = compareFiles("output2.txt", "expected2.txt");
    if (result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return result;
}

int test3()
{
    printf("\nTest 3^ Empty cells\n");
    int result = compareFiles("output3.txt", "expected3.txt");
    if (result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return result;
}

int test4()
{
    printf("\nTest 4^ Too many cells\n");
    int result = compareFiles("output4.txt", "expected4.txt");
    if (result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    return result;
}

int main(int argc, char* argv[])
{
    printf("Running the tests:\n");
    test1();
    test2();
    test3();
    test4();

    return 0;
}
