#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>

struct myNumbers {
	std::vector<int> numbers;
	int max;
	int min;
	double average;
};
DWORD WINAPI average(LPVOID lpParam)
{
	myNumbers* pointer = (myNumbers*)lpParam;
	int sum = 0;
	for (int i = 0; i < pointer->numbers.size(); i++) {
		sum += pointer->numbers[i];
		Sleep(12);
	}
	pointer->average = sum / pointer->numbers.size();
	std::cout << "������� �������������� ��������� �������: " << pointer->average << ". ��� ������ ��������� ������� �������� ����� ��������� ����.\n";
	return 0;
}
DWORD WINAPI min_max(LPVOID lpParam)
{
	myNumbers* pointer = (myNumbers*)lpParam;
	pointer->min = pointer->max = pointer->numbers[0];
	for (int i = 0; i < pointer->numbers.size(); i++) {
		if (pointer->numbers[i] > pointer->max)
			pointer->max = pointer->numbers[i];
		else if (pointer->numbers[i] < pointer->min)
			pointer->min = pointer->numbers[i];
		Sleep(7);
	}
	std::cout << "����������� ������� �������: " << pointer->min << ". ������������ ������� �������: " << pointer->max << ".\n";
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "������� ������ �������\n";
	int size;
	std::cin >> size;
	while (true) {
		if (size <= 0) {
			std::cout << "������ ������ ���� ������ ����, ��������� ����.\n";
			std::cin >> size;
		}
		else
			break;
	}
	std::vector<int> numbers;
	std::cout << "������� ������\n";
	for (int i = 0; i < size; i++) {
		int buffer;
		std::cin >> buffer;
		numbers.push_back(buffer);
	}
	myNumbers* toThread = new myNumbers();
	toThread->numbers = numbers;
	HANDLE min_maxHandle = CreateThread(
		NULL,
		0,
		min_max,
		toThread,
		0,
		NULL);
	WaitForSingleObject(min_maxHandle, INFINITE);
	CloseHandle(min_maxHandle);
	HANDLE averageHandle = CreateThread(
		NULL,
		0,
		average,
		toThread,
		0,
		NULL);
	WaitForSingleObject(averageHandle, INFINITE);
	CloseHandle(averageHandle);
	std::cout << "�������� �� ������:\n";
	for (auto number : toThread->numbers)
		std::cout << number << " ";
	std::cout << "\n";
	std::cout << "�������� ����� ������:\n";
	for (int i = 0; i < toThread->numbers.size(); i++) {
		if (toThread->numbers[i] == toThread->max || toThread->numbers[i] == toThread->min)
			toThread->numbers[i] = toThread->average;
		std::cout << toThread->numbers[i] << " ";
	}
	std::cout << "\n";
	return 0;
}