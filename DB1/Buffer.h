#pragma once
#include <vector>
#include "Record.h"
#include <iostream>
#include <fstream>
#define BUFFER_SIZE 1053

class Buffer {
    const char endRecChar = ':';
    int lastEndRecCharIndex = 0;
    char buffer[BUFFER_SIZE];
    const int maxNumOfRecordsInBuffer = BUFFER_SIZE / RECORD_SIZE_IN_FILE;
    std::vector<Record> recordVector;
    bool isSorted = true;

public:
    Buffer();
    void nullTerminateTheBuffer(int index);
    void fillBuffer(char* content,int size);
    void emptyBuffer();
    char* getBufferTable();
    void recordListToFile(std::vector<Record>& resultRecords);
    void recordListToFile(std::vector<Record>& resultRecords, Buffer* buffer1, Buffer* buffer2);
    void recordListToFile2(std::vector<Record>& resultRecords);
    std::streampos readRecordsFormated();
    void saveRecordsFormated(std::vector<Record> recordVector);
    std::string recordListToString() const;
    void printBuffer() const;
    std::vector<Record> getRecordVector() const;
    void printRecordVector() const;
    bool getIsSorted();
    void setIsSorted(bool isSorted);
    int readTimes = 0;
    int saveTimes = 0;
    float lastAvg = -1.0;
};
