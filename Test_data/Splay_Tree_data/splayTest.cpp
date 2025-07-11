#include <iostream>
#include "SplayTree.h"
#include "Generator.h"
#include <chrono>
#include <fstream>

using clocks = std::chrono::high_resolution_clock;
using nanoseconds = std::chrono::nanoseconds;

void timeTest() {

	TreeInt s;
	std::ofstream outFile("C:/Users/Student/Documents/Tree/test.csv");
	for (int i = 0; i < 10000; ++i) {
		s.insert(i);
	}
	for (int i = 0; i < 10000; ++i) {
		auto start = clocks::now();
		s.find(9999);
		auto elapsed = clocks::now() - start; // текущее время после выполнения
		long long nanoSec = std::chrono::duration_cast<nanoseconds>(elapsed).count();

		outFile << nanoSec << std::endl; // записываем время работы в файл
	}
}

int main() {
	timeTest();
}