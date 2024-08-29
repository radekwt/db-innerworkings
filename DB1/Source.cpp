#include <iostream>
#include <fstream>
#include "Tape.h"
#include <chrono>
#include <thread>
using namespace std;

int main() {
	Tape tape("input.txt");
	int N =	1000;
	tape.fillTapeRandom(N);
	//tape.printTape();
	//cout << tape.isSorted();
	//cout << "Worst case READ/WRITE: " << 4 * N * (log2(N)) / (BUFFER_SIZE / RECORD_SIZE_IN_FILE) << endl;;
	//cout << "Worst case phases: " << log2(N) << endl;
	Tape tape1("tape1.txt");
	Tape tape2("tape2.txt");
	Tape tape3("tape3.txt");
	int phases = 0;
	bool oneToTwo = true;
	int res = 0;
	tape.getBuffer()->readTimes = 0;
	tape.getBuffer()->saveTimes = 0;
	while (true) {
		//system("cls");
		if (oneToTwo) {
			if (tape.fromOneToTwo(&tape1, &tape2) == -1) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else
			{
				oneToTwo = false;
			}
		}
		else {
			res = tape.fromTwoToOne(&tape1, &tape2);
			if (res == 2) {
				break;
			}
			else if (res == -1) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else {
				phases++;
				oneToTwo =true;
			}
		}
		/*cout << endl << "Tape: " << endl;
		tape.printTape();
		cout << endl << "Tape1: " << endl;
		tape1.printTape();
		cout << endl << "Tape2: " << endl;
		tape2.printTape();
		system("cls");*/
		/*cout << endl << "Tape1: " << endl;
		tape1.printTape();
		cout << endl << "Tape2: " << endl;
		tape2.printTape();
		cout << endl << "Tape: " << endl;
		tape.printTape();*/
	
	}
	//system("cls");
	//tape.printTape();
	/*cout << endl << "tape0: " << endl;
	tape.printTape();
	cout << endl << "tape1: " << endl;
	tape1.printTape();
	cout << endl << "tape2: " << endl;
	tape2.printTape();*/
	int saveTimes = tape.getBuffer()->saveTimes + tape1.getBuffer()->saveTimes + tape2.getBuffer()->saveTimes;
	int readTimes = tape.getBuffer()->readTimes + tape1.getBuffer()->readTimes + tape2.getBuffer()->readTimes;
	cout << "Saved times: " << saveTimes << endl;
	cout << "Saved times0: " << tape.getBuffer()->saveTimes << endl;
	cout << "Saved times1: " << tape1.getBuffer()->saveTimes << endl;
	cout << "Saved times2: " << tape2.getBuffer()->saveTimes << endl;
	cout << "Read times: " << readTimes << endl;
	cout << "Read times0: " << tape.getBuffer()->readTimes << endl;
	cout << "Read times1: " << tape1.getBuffer()->readTimes << endl;
	cout << "Read times2: " << tape2.getBuffer()->readTimes << endl;
	cout << "READ/WRITE: " << saveTimes+readTimes << endl;
	cout << "Worst case READ/WRITE: " << 4*N*(log2(N))/(BUFFER_SIZE/RECORD_SIZE_IN_FILE) << endl;
	cout << "Phases: "<<phases << endl;
	cout << "Worst case phases: " << log2(N) << endl;
	return 0;
}