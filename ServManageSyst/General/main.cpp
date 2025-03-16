#include <iostream>
#include <clocale>
#include <string>
#include <Windows.h>

int main() {
	setlocale(LC_ALL, "rus");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	return 0;
}