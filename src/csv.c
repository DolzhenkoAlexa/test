#include "csv.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 10000 // Максимальный размер файла

Table* csvParser(FILE* file)
{
    Table* table = calloc(1, sizeof(Table));
    if (!table)
        return NULL;

    // Проверка на пустой файл
    int firstCharacter = fgetc(file);
    if (firstCharacter == EOF) {
        free(table);
        return NULL;
    }
    ungetc(firstCharacter, file);

    // Выделяем память фиксированного размера
    table->data = malloc(MAX_BUFFER_SIZE);
    if (!table->data) {
        free(table);
        return NULL;
    }

    // Читаем файл
    int character;
    int pos = 0;
    while ((character = fgetc(file)) != EOF && pos < MAX_BUFFER_SIZE - 1) {
        table->data[pos++] = (char)character;
    }
    table->data[pos] = '\0';
    table->dataLength = pos;

    // Считаем количество колонок по первой строке
    int columnCount = 1;
    for (int i = 0; i < table->dataLength && table->data[i] != '\n'; i++) {
        if (table->data[i] == ',') {
            columnCount++;
        }
    }

    table->columnCount = columnCount;
    table->columnWidths = calloc(columnCount, sizeof(int));
    if (!table->columnWidths) {
        free(table->data);
        free(table);
        return NULL;
    }

    // Считаем ширину колонок
    int currentColumn = 0;
    int currentCellLength = 0;

    for (int i = 0; i < table->dataLength; i++) {
        char currentCharacter = table->data[i];

        if (currentCharacter != ',' && currentCharacter != '\n') {
            currentCellLength++;
        } else {
            if (currentCellLength > table->columnWidths[currentColumn]) {
                table->columnWidths[currentColumn] = currentCellLength;
            }

            if (currentCharacter == ',') {
                currentColumn++;
            } else if (currentCharacter == '\n') {
                currentColumn = 0;
            }
            currentCellLength = 0;
        }
    }

    if (currentCellLength > 0) {
        if (currentCellLength > table->columnWidths[currentColumn]) {
            table->columnWidths[currentColumn] = currentCellLength;
        }
    }

    return table;
}

static void printHeader(FILE* output, const int* columnWidths, int columnCount)
{
    fputc('+', output);
    for (int i = 0; i < columnCount; i++) {
        for (int j = 0; j < columnWidths[i] + 2; j++) {
            fputc('=', output);
        }
        fputc('+', output);
    }
    fputc('\n', output);
}

static void printMiddle(FILE* output, const int* columnWidths, int columnCount)
{
    fputc('+', output);
    for (int i = 0; i < columnCount; i++) {
        for (int j = 0; j < columnWidths[i] + 2; j++) {
            fputc('-', output);
        }
        fputc('+', output);
    }
    fputc('\n', output);
}

static int isNumeric(const char* string, int length)
{
    int hasDigit = 0;
    for (int i = 0; i < length; i++) {
        if (isdigit(string[i])) {
            hasDigit = 1;
        } else if (string[i] != '.' && string[i] != '-') {
            return 0;
        }
    }
    return hasDigit;
}

static void printCell(FILE* output, const char* string, int length, int width, int isNumber)
{
    if (isNumber) {
        fprintf(output, " %*.*s |", width, length, string);
    } else {
        fprintf(output, " %-*.*s |", width, length, string);
    }
}

static void printEmptyCell(FILE* output, int width)
{
    fprintf(output, " %*s |", width, "");
}

void tableMaker(FILE* output, Table* table)
{
    if (table->columnCount == 0 || table->dataLength == 0) {
        return;
    }

    printHeader(output, table->columnWidths, table->columnCount);

    int currentPosition = 0;
    int isFirstRow = 1;

    while (currentPosition < table->dataLength) {
        if (table->data[currentPosition] == '\n') {
            currentPosition++;
            continue;
        }

        fputc('|', output);
        int currentColumn = 0;

        while (currentPosition < table->dataLength && table->data[currentPosition] != '\n') {
            if (currentColumn >= table->columnCount) {
                while (currentPosition < table->dataLength && table->data[currentPosition] != '\n') {
                    currentPosition++;
                }
                break;
            }

            int cellStart = currentPosition;

            while (currentPosition < table->dataLength && table->data[currentPosition] != ',' && table->data[currentPosition] != '\n') {
                currentPosition++;
            }

            int cellLength = currentPosition - cellStart;
            int isNumber = isNumeric(table->data + cellStart, cellLength);

            printCell(output, table->data + cellStart, cellLength,
                table->columnWidths[currentColumn], isNumber);
            currentColumn++;

            if (currentPosition < table->dataLength && table->data[currentPosition] == ',') {
                currentPosition++;
            }
        }

        while (currentColumn < table->columnCount) {
            printEmptyCell(output, table->columnWidths[currentColumn]);
            currentColumn++;
        }

        if (currentPosition < table->dataLength && table->data[currentPosition] == '\n') {
            currentPosition++;
        }

        if (isFirstRow) {
            fputc('\n', output);
            printMiddle(output, table->columnWidths, table->columnCount);
            isFirstRow = 0;
        } else {
            fprintf(output, "\n");
            printMiddle(output, table->columnWidths, table->columnCount);
        }
    }
}

void freeTable(Table* table)
{
    if (table) {
        free(table->data);
        free(table->columnWidths);
        free(table);
    }
}
