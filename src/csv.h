#pragma once

#include <stdio.h>

typedef struct {
    char* data;
    int* columnWidths;
    int columnCount;
    int dataLength;
} Table;

Table* csvParser(FILE* f);
void tableMaker(FILE* out, Table* t);
void freeTable(Table* table);
