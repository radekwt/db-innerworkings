#pragma once
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include "Buffer.h"
#include "Record.h"
class Tape{
	std::fstream file;
	std::string filePath;
	Buffer* buffer;
	unsigned int placeInFile = 0;
public:
	Tape(std::string filePath);
	void resetTape();
	void fillTapeRandom(int amount);
	void fillTapeSorted(int amount);
	Buffer* getBuffer();
	int fromOneToTwo(Tape* tape1, Tape* tape2);
	int fromTwoToOne(Tape* tape1, Tape* tape2);
	void printTape();

};

