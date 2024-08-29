#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#define RECORD_SIZE_IN_FILE 20
class Record
{
protected:
    short int grades[3];
    int id;
    float avg;
public:
    Record();
    Record(int studentId, short int grade1, short int grade2, short int grade3);
    Record copyRecord() const;
    float getGradeAvg() const;
    int compareRecordAvgs(const Record& other) const;
    int compareRecordAvgs(const float average) const;
    int getId() const;
    short int getGrade(int index) const;
    std::string recordToString() const;
    std::string recordToFileString() const;
    friend class Buffer;

};

