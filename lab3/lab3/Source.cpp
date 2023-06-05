#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

CRITICAL_SECTION criticalSection;
std::vector<int> numbers;
HANDLE continueEvent;
HANDLE startupEvent;

struct toThread {
	int threadNumber;
	HANDLE& busyEvent;
	toThread(int _threadNumber, HANDLE& _busyEvent) :
		busyEvent(_busyEvent)
	{
		threadNumber = _threadNumber;
	}
};

DWORD WINAPI marker(LPVOID lpParam) {
	toThread* pointer = (toThread*)lpParam;
	srand(pointer->threadNumber);
	std::vector<int> indexes;
	int temp = rand() % (numbers.size());
	int changed;
	do {
		changed = 0;
		SignalObjectAndWait(pointer->busyEvent, startupEvent, INFINITE, FALSE);		
		while (true) {
			EnterCriticalSection(&criticalSection);
			if (numbers.at(temp) != 0) {
				LeaveCriticalSection(&criticalSection);
				break;
			}
			Sleep(5);
			numbers.at(temp) = pointer->threadNumber + 1;
			indexes.push_back(temp);
			LeaveCriticalSection(&criticalSection);
			Sleep(5);
			temp = rand() % (numbers.size());
			changed++;
		}
		std::string output = "����� ����� " + std::to_string(pointer->threadNumber + 1) + ". �������� ���������: " + std::to_string(changed) + " . ������ ��������, ������� ���������� ��������: " + std::to_string(temp) + ". ��������.\n";
		std::cout << output;
		SignalObjectAndWait(pointer->busyEvent, continueEvent, INFINITE, FALSE);
	} while (WaitForSingleObject(pointer->busyEvent, 0) == WAIT_TIMEOUT);
	for (auto index : indexes) {
		numbers.at(index) = 0;
	}
	delete pointer;
	return 0;
}

int main() {
	setlocale(LC_ALL, "Russian");
	std::cout << "������� ������ �������\n";
	int size, threadsNumber;
	do {
		std::cin >> size;
	} while (size <= 0);
	numbers = std::vector<int>(size);
	std::cout << "������� ���������� �������\n";
	do {
		std::cin >> threadsNumber;
	} while (threadsNumber <= 0);
	HANDLE* handles = new HANDLE[threadsNumber];
	HANDLE* busyEvents = new HANDLE[threadsNumber];
	startupEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	continueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&criticalSection); 
	for (int i = 0; i < threadsNumber; i++) {
		busyEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		handles[i] = CreateThread(NULL, 0, marker, new toThread(i, busyEvents[i]), 0, NULL);
	}
	while (WaitForMultipleObjects(threadsNumber, handles, TRUE, 0) == WAIT_TIMEOUT) { 
		WaitForMultipleObjects(threadsNumber, busyEvents, TRUE, INFINITE);
		ResetEvent(continueEvent);
		SetEvent(startupEvent);
		WaitForMultipleObjects(threadsNumber, busyEvents, TRUE, INFINITE);
		ResetEvent(startupEvent);
		std::cout << "������:\n";
		for (auto number : numbers) {
			std::cout << number << " ";
		}
		std::cout << "\n";
		DWORD exitCode;
		while (true) {
			std::cout << "������� ����� ������ (�� 1 �� " << threadsNumber << "), �� ����� ��������.\n";
			do {
				std::cin >> size;
			} while (size < 1 || size > threadsNumber);
			if (GetExitCodeThread(handles[size - 1], &exitCode)) {
				if (exitCode == STILL_ACTIVE) {
					busyEvents[size - 1] = CreateEvent(NULL, TRUE, TRUE, NULL); 
					SetEvent(continueEvent);
					WaitForSingleObject(handles[size - 1], INFINITE);
					std::cout << "������:\n";
					for (auto number : numbers) {
						std::cout << number << " ";
					}
					std::cout << "\n";
					break;
				}
				else {
					std::cout << "����� ��� �������� ���� ������.\n";
				}
			}
			else {
				std::cout << "���������� ��������� ������ ������.\n";
			}
		}
		std::cout << "---\n";
	}
	for (int i = 0; i < threadsNumber; i++) {
		CloseHandle(handles[i]);
	}
	return 0;
}