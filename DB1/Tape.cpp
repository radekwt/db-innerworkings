#include "Tape.h"

Tape::Tape(std::string filePath) : filePath(filePath)
{
    buffer = new Buffer();
}
Buffer* Tape::getBuffer()
{
    return this->buffer;
}

void Tape::resetTape()
{
    this->buffer->lastAvg = -1.0;
    this->placeInFile = 0;
    buffer->emptyBuffer();
}


int Tape::fromOneToTwo(Tape* tape1, Tape* tape2) {
    std::ifstream file(this->filePath, std::ios::in | std::ios::binary);
    std::ofstream outputFile1(tape1->filePath, std::ios::out | std::ios::binary);
    std::ofstream outputFile2(tape2->filePath, std::ios::out | std::ios::binary);
    if (!file.is_open() || !outputFile1.is_open() || !outputFile2.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return -1;
    }
    this->resetTape();
    std::vector<Record> recordVector1;
    std::vector<Record> recordVector2;
    int chunkSize = BUFFER_SIZE;
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    int intFileSize = static_cast<int>(fileSize);
    file.seekg(0, std::ios::beg);
    bool tape1Turn = true;
    while (true) {
        this->buffer->emptyBuffer();
        if (intFileSize - this->placeInFile < chunkSize) {
            chunkSize = intFileSize - this->placeInFile;
        }
        file.seekg(this->placeInFile, std::ios::beg);
        file.read(this->buffer->getBufferTable(), chunkSize);
        this->placeInFile += this->buffer->readRecordsFormated();
        while (!this->buffer->getRecordVector().empty()) {
            if (tape1Turn) {
                this->buffer->recordListToFile(recordVector1);
                if (recordVector1.size() == int(BUFFER_SIZE / RECORD_SIZE_IN_FILE)) {
                    tape1->buffer->saveRecordsFormated(recordVector1);
                    outputFile1 << tape1->buffer->getBufferTable();
                    recordVector1.clear();
                }
                if (this->buffer->lastAvg==-1) {
                    tape1Turn = false;
                }
            }
            else {
                this->buffer->recordListToFile(recordVector2);
                if (recordVector2.size() == int(BUFFER_SIZE / RECORD_SIZE_IN_FILE)) {
                    tape2->buffer->saveRecordsFormated(recordVector2);
                    outputFile2 << tape2->buffer->getBufferTable();
                    recordVector2.clear();
                }
                if (this->buffer->lastAvg == -1) {
                    tape1Turn = true;
                }
            }
        }
        if (this->placeInFile >= fileSize || file.tellg() == -1) {
            if (!recordVector1.empty()) {
                tape1->buffer->saveRecordsFormated(recordVector1);
                outputFile1 << tape1->buffer->getBufferTable();
                recordVector1.clear();
            }
            if (!recordVector2.empty()) {
                tape2->buffer->saveRecordsFormated(recordVector2);
                outputFile2 << tape2->buffer->getBufferTable();
                recordVector2.clear();
            }
            break;
        }
    }
    file.close();
    outputFile1.close();
    outputFile2.close();
    return 1;
}
int Tape::fromTwoToOne(Tape* tape1, Tape* tape2) {
    std::ofstream file(this->filePath, std::ios::out | std::ios::binary);
    std::ifstream tape1File(tape1->filePath, std::ios::in | std::ios::binary);
    std::ifstream tape2File(tape2->filePath, std::ios::in | std::ios::binary);

    if (!file.is_open() ) {
        std::cerr << "Failed to open the file. 3" << std::endl;
        return -1;
    }
    if ( !tape1File.is_open()) {
        std::cerr << "Failed to open the file. 1" << std::endl;
        return -1;
    }
    if ( !tape2File.is_open()) {
        std::cerr << "Failed to open the file. 2" << std::endl;
        return -1;
    }
    tape1->resetTape();
    tape2->resetTape();

    std::vector<Record> recordVector;
    int tape1FileChunkSize = BUFFER_SIZE;
    tape1File.seekg(0, std::ios::end);
    std::streampos tape1FileSize = tape1File.tellg();
    int inttape1FileSize = static_cast<int>(tape1FileSize);
    tape1File.seekg(0, std::ios::beg);

    int tape2FileChunkSize = BUFFER_SIZE;
    tape2File.seekg(0, std::ios::end);
    std::streampos tape2FileSize = tape2File.tellg();
    int inttape2FileSize = static_cast<int>(tape2FileSize);
    tape2File.seekg(0, std::ios::beg);

    bool isFinishedTape1 = false;
    bool isFinishedTape2 = false;
    bool tape1Turn = true;
    tape1->buffer->emptyBuffer();
    tape2->buffer->emptyBuffer();
    tape1->buffer->printRecordVector();
    tape2->buffer->printRecordVector();
    while (1) {
        if (tape1->buffer->getRecordVector().empty()) {
            tape1->buffer->emptyBuffer();
            if (inttape1FileSize - tape1->placeInFile < tape1FileChunkSize) {
                tape1FileChunkSize = inttape1FileSize - tape1->placeInFile;
            }
            tape1File.seekg(tape1->placeInFile, std::ios::beg);
            tape1File.read(tape1->buffer->getBufferTable(), tape1FileChunkSize);
            tape1->placeInFile += tape1->buffer->readRecordsFormated();
        }
        if (tape2->buffer->getRecordVector().empty()) {
            tape2->buffer->emptyBuffer();
            if (inttape2FileSize - tape2->placeInFile < tape2FileChunkSize) {
                tape2FileChunkSize = inttape2FileSize - tape2->placeInFile;
            }
            tape2File.seekg(tape2->placeInFile, std::ios::beg);
            tape2File.read(tape2->buffer->getBufferTable(), tape2FileChunkSize);
            tape2->placeInFile += tape2->buffer->readRecordsFormated();
        }
        while (!tape1->buffer->getRecordVector().empty() && !tape2->buffer->getRecordVector().empty()) {
            this->buffer->recordListToFile(recordVector,tape1->buffer, tape2->buffer);
            if (recordVector.size() == int(BUFFER_SIZE / RECORD_SIZE_IN_FILE)) {
                this->buffer->saveRecordsFormated(recordVector);
                file << this->buffer->getBufferTable();
                recordVector.clear();
            }
        }

        if (tape1->placeInFile >= inttape1FileSize  || tape1File.tellg() == -1) {
            isFinishedTape1 = true;
            tape2->buffer->recordListToFile2(recordVector);
            if (recordVector.size() == int(BUFFER_SIZE / RECORD_SIZE_IN_FILE)) {
                this->buffer->saveRecordsFormated(recordVector);
                file << this->buffer->getBufferTable();
                recordVector.clear();
            }
            else if (tape2->placeInFile >= inttape2FileSize) {
                this->buffer->saveRecordsFormated(recordVector);
                file << this->buffer->getBufferTable();
                recordVector.clear();
                if (tape1->buffer->getRecordVector().empty() && tape2->buffer->getRecordVector().empty()) {
                    break;
                }
            }
        }
        if (tape2->placeInFile >= inttape2FileSize || tape2File.tellg() == -1) {
            isFinishedTape2 = true;
            tape1->buffer->recordListToFile2(recordVector);
            if (recordVector.size() == int(BUFFER_SIZE / RECORD_SIZE_IN_FILE)) {
                this->buffer->saveRecordsFormated(recordVector);
                file << this->buffer->getBufferTable();
                recordVector.clear();
            }
            else if (tape1->placeInFile >= inttape1FileSize) {
                this->buffer->saveRecordsFormated(recordVector);
                file << this->buffer->getBufferTable();
                recordVector.clear();
                if (tape1->buffer->getRecordVector().empty() && tape2->buffer->getRecordVector().empty()) {
                    break;
                }
            }
        }
    }
    if (tape1->placeInFile == 0 || tape2->placeInFile == 0) {
        return 2;
    }
    file.close();
    tape1File.close();
    tape2File.close();
    return 1;
}

void Tape::printTape()
{
    this->resetTape();
    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    int chunkSize = BUFFER_SIZE;
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    int intFileSize = static_cast<int>(fileSize);
    file.seekg(0, std::ios::beg);

    while (true) {
        this->buffer->emptyBuffer();
        if (intFileSize - placeInFile < chunkSize) {
            chunkSize = intFileSize - placeInFile;
        }
        file.seekg(placeInFile, std::ios::beg);
        file.read(this->buffer->getBufferTable(), chunkSize);
        placeInFile += this->buffer->readRecordsFormated();
        this->buffer->printRecordVector();
        if (placeInFile >= fileSize || file.tellg() == -1) {
            break;
        }

    }
    file.close();
    return;
}
void Tape::fillTapeRandom(int amount)
{
    std::ofstream outputFile(this->filePath, std::ios::out | std::ios::binary);
    int index = 0;
    std::vector<Record> recordVector;
    if (outputFile.is_open()) {
        while (index<amount) {
            for (int i = 0; i < (BUFFER_SIZE / RECORD_SIZE_IN_FILE); i++) {
                if (index >= amount) {
                    break;
                }
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> gradeDistribution(0, 100);
                int studentId = index;
                short int grade1 = gradeDistribution(generator);
                short int grade2 = gradeDistribution(generator);
                short int grade3 = gradeDistribution(generator);
                Record record(studentId + 1, grade1, grade2, grade3);
                recordVector.push_back(record);
                index++;
            }
            this->buffer->saveRecordsFormated(recordVector);
            outputFile << this->buffer->getBufferTable();
            recordVector.clear();
        }
        outputFile.close();
    }
    else {
        std::cerr << "Failed to open " << this->filePath << " for writing." << std::endl;
    }
}
void Tape::fillTapeSorted(int amount)
{
    std::ofstream outputFile(this->filePath, std::ios::out | std::ios::binary);
    int index = 0;
    std::vector<Record> recordVector;
    if (outputFile.is_open()) {
        while (index<amount) {
            for (int i = 0; i < (BUFFER_SIZE / RECORD_SIZE_IN_FILE); i++) {
                if (index >= amount) {
                    break;
                }
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> gradeDistribution(0, 100);
                int studentId = index;
                short int grade1 = index;
                short int grade2 = index;
                short int grade3 = index;
                Record record(studentId + 1, grade1, grade2, grade3);
                recordVector.push_back(record);
                index++;
            }
            this->buffer->saveRecordsFormated(recordVector);
            outputFile << this->buffer->getBufferTable();
            recordVector.clear();
        }
        outputFile.close();
        std::cout << "Records written to " << this->filePath << std::endl;
    }
    else {
        std::cerr << "Failed to open " << this->filePath << " for writing." << std::endl;
    }
}
