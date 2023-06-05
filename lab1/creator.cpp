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
		std::cout << "Неверные аргументы, creator завершает работу\n";
	else {
		std::ofstream fout(argv[0], std::ios_base::trunc | std::ios_base::binary | std::ios_base::out);
		if (fout.is_open()) {
			int notesCount = std::atoi(argv[1]);
			employee temp;
			for (int i = 0; i < notesCount; i++) {
				std::cout << "Введите идентификационный номер сотрудника\n";
				std::cin >> temp.num;
				std::cout << "Введите имя сотрудника\n";
				std::cin >> temp.name;
				std::cout << "Введите количество отработанных сотрудником часов\n";
				std::cin >> temp.hours;
				fout << temp.num << " " << temp.name << " " << temp.hours << std::endl;
				std::cout << "Добавлено, осталось добавить " << notesCount - i - 1 << " строк\n";
			}
			fout.close();
		}
		else
			std::cout << "Файл не может быть открыт, creator завершает работу\n";
	}
	return 0;
}