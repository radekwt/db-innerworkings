#include "Record.h"
Record::Record() : id(0), avg(-1.0) {
    for (int i = 0; i < 3; i++) {
        grades[i] = 0;
    }
}

// Parameterized constructor
Record::Record(int studentId, short int grade1, short int grade2, short int grade3) : id(studentId) {
    grades[0] = grade1;
    grades[1] = grade2;
    grades[2] = grade3;
    avg = getGradeAvg();
}

// Member function to calculate the average of grades
float Record::getGradeAvg() const {
    float sum = 0.0;
    for (int i = 0; i < 3; i++) {
        sum += grades[i];
    }
    return sum / 3.0f;
}

// Member function to get the student's ID
int Record::getId() const {
    return id;
}

// Member function to get a specific grade
short int Record::getGrade(int index) const {
    if (index >= 0 && index < 3) {
        return grades[index];
    }
    return -1; // You can handle invalid index as per your requirements.
}

int Record::compareRecordAvgs(const Record& other) const {
    // Assuming avg is a data member representing the average value
    float diff = avg - other.avg;

    // Use a small epsilon to account for floating-point imprecision
    const float epsilon = 1e-6f; 

    if (std::abs(diff) < epsilon) {
        return 0; // Averages are considered equal
    }
    else if (diff < 0) {
        return -1; // The average of this record is less than the other
    }
    else {
        return 1; // The average of this record is greater than the other
    }
}
int Record::compareRecordAvgs(const float average) const {
    // Assuming avg is a data member representing the average value
    float diff = this->avg - average;

    // Use a small epsilon to account for floating-point imprecision
    const float epsilon = 1e-6f;

    if (std::abs(diff) < epsilon) {
        return 0; // Averages are considered equal
    }
    else if (diff < 0) {
        return -1; // The average of this record is less than the other
    }
    else {
        return 1; // The average of this record is greater than the other
    }
}


std::string Record::recordToString() const
{
    std::string recordStr = "Student ID: " + std::to_string(id) + ", Grades: ";
    for (int i = 0; i < 3; i++) {
        recordStr += std::to_string(grades[i]);
        if (i < 2) {
            recordStr += ", ";
        }
    }
    recordStr += ", Average: " + std::to_string(avg);
    return recordStr;
}

std::string Record::recordToFileString() const
{
    // Create formatted strings for each field with a fixed width of 3 characters
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(10) << std::right << id;
    oss << std::setfill('0') << std::setw(3) << std::right << grades[0];
    oss << std::setfill('0') << std::setw(3) << std::right << grades[1];
    oss << std::setfill('0') << std::setw(3) << std::right << grades[2];
    oss << "\n"; // Add a newline character at the end
    return oss.str();
}





