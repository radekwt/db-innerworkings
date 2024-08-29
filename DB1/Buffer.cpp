#include "Buffer.h"
#include <iostream>
#include <sstream>

Buffer::Buffer()
{
}

void Buffer::nullTerminateTheBuffer(int index)
{
    buffer[index] = '\0';
}
void Buffer::fillBuffer(char* content,int size)
{
    for (int i = 0; i < BUFFER_SIZE; i++) {
        this->buffer[i] = content[i];
    }
}
void Buffer::recordListToFile2(std::vector<Record>& resultRecords) { // Vector to store records

    auto iter = recordVector.begin();

    while (iter != recordVector.end() && resultRecords.size() < maxNumOfRecordsInBuffer) {
        const Record& record = *iter;
        resultRecords.push_back(record); // Add record to the result vector
        iter = recordVector.erase(iter); // Erase the record from the original vector
    }
 // Return the vector containing records
}

void Buffer::recordListToFile(std::vector<Record>& resultRecords)
{
    auto iter = recordVector.begin(); // Iterator for removing elements
    while (iter != recordVector.end() && resultRecords.size() < maxNumOfRecordsInBuffer) {
        const Record& record = *iter;
        if (this->lastAvg > record.getGradeAvg()) {
            this->lastAvg = -1.0;
            break;
        }
        resultRecords.push_back(record); // Add the record to the new vector
        this->lastAvg = record.getGradeAvg();
        iter = recordVector.erase(iter); // Erase the record from the original vector
    }
}

void Buffer::recordListToFile(std::vector<Record> &resultRecords, Buffer* buffer1, Buffer* buffer2) {


    auto iterTape1 = buffer1->recordVector.begin();
    auto iterTape2 = buffer2->recordVector.begin();

    while (iterTape1 != buffer1->recordVector.end() && iterTape2 != buffer2->recordVector.end() && resultRecords.size() < maxNumOfRecordsInBuffer) {
        const Record& recordTape1 = *iterTape1;
        const Record& recordTape2 = *iterTape2;
        if (buffer1->lastAvg > recordTape1.getGradeAvg() && buffer2->lastAvg > recordTape2.getGradeAvg()) {
            buffer1->lastAvg = -1.0;
            buffer2->lastAvg = -1.0;
        }
        if (recordTape1.getGradeAvg() < recordTape2.getGradeAvg() && buffer1->lastAvg<=recordTape1.getGradeAvg() || buffer2->lastAvg > recordTape2.getGradeAvg()) {
            if (buffer1->lastAvg > recordTape1.getGradeAvg()) {
                buffer2->lastAvg = -1.0;
            }
            resultRecords.push_back(recordTape1); // Add record to the result vector
            buffer1->lastAvg = recordTape1.getGradeAvg();
            iterTape1 = buffer1->recordVector.erase(iterTape1);
        }
        else {
            if (buffer2->lastAvg > recordTape2.getGradeAvg()) {
                buffer1->lastAvg = -1.0;
            }
            resultRecords.push_back(recordTape2); // Add record to the result vector
            buffer2->lastAvg = recordTape2.getGradeAvg();
            iterTape2 = buffer2->recordVector.erase(iterTape2);
        }
    }
 // Return the vector containing qualifying records
}

std::streampos Buffer::readRecordsFormated() {
    this->recordVector.clear();
    this->readTimes++;

    constexpr size_t recordSize = RECORD_SIZE_IN_FILE;

    const char* bufferPtr = buffer;

    for (size_t i = 0; i < BUFFER_SIZE; i += recordSize) {
        char recordBuffer[recordSize]; 

        memcpy(recordBuffer, bufferPtr, recordSize);
        bufferPtr += recordSize;
        size_t recordEnd = recordSize;
        if (recordEnd >= 1 && recordBuffer[recordEnd - 1] == '\x0A') {
            recordBuffer[recordEnd - 1] = '\0';
        }

        int studentId;
        short int grade1, grade2, grade3;

        if (sscanf(recordBuffer, "%10d%3hd%3hd%3hd", &studentId, &grade1, &grade2, &grade3) == 4) {
            Record record(studentId, grade1, grade2, grade3);
            this->recordVector.push_back(record);
        }
        else {
            break; 
        }

    }
    return static_cast<std::streampos>(recordVector.size()) * recordSize;
}
void Buffer::saveRecordsFormated(std::vector<Record> recordVector) {
    this->emptyBuffer();
    std::string recordString;
    int index = 0;
    this->saveTimes++;
    for (const auto& record : recordVector) {
        recordString += record.recordToFileString();
    }
    

    const char* recordCString = recordString.c_str(); 

    strncpy(this->buffer, recordCString, BUFFER_SIZE);
    return;
}

std::string Buffer::recordListToString() const{
    std::string result;

    for (const auto& record : recordVector) {
        result += record.recordToFileString() + '\n';
    }

    return result;
}

void Buffer::printBuffer() const
{
    for (int i = 0; i < BUFFER_SIZE; i++) {
        std::cout << this->buffer[i];
    }
    std::cout << std::endl;
}

// Add getter for the recordList if needed
std::vector<Record> Buffer::getRecordVector() const {
    return this->recordVector;
}

void Buffer::printRecordVector() const
{
    for (const auto& record : recordVector) {
        std::cout<<record.recordToString()<<std::endl;
    }
}
bool Buffer::getIsSorted()
{
    return isSorted;
}
void Buffer::setIsSorted(bool isSorted)
{
    this->isSorted = isSorted;
}
char* Buffer::getBufferTable() {
    return buffer;
}
void Buffer::emptyBuffer() {
    std::memset(buffer, 0, BUFFER_SIZE);
    this->recordVector.clear();
}
