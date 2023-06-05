#include <iostream>
#include <fstream>
#include <Windows.h>
struct employee
{
	int num;
	char name[10];
	double hours;
};
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argc != 2)
		std::cout << "�������� ���������, creator ��������� ������\n";
	else {
		std::ofstream fout(argv[0], std::ios_base::trunc | std::ios_base::binary | std::ios_base::out);
		if (fout.is_open()) {
			int notesCount = std::atoi(argv[1]);
			employee temp;
			for (int i = 0; i < notesCount; i++) {
				std::cout << "������� ����������������� ����� ����������\n";
				std::cin >> temp.num;
				std::cout << "������� ��� ����������\n";
				std::cin >> temp.name;
				std::cout << "������� ���������� ������������ ����������� �����\n";
				std::cin >> temp.hours;
				fout << temp.num << " " << temp.name << " " << temp.hours << std::endl;
				std::cout << "���������, �������� �������� " << notesCount - i - 1 << " �����\n";
			}
			fout.close();
		}
		else
			std::cout << "���� �� ����� ���� ������, creator ��������� ������\n";
	}
	return 0;
}