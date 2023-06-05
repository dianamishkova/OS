#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <sstream>
struct employee
{
	int num;
	std::string name;
	double hours;
	long double total;
};
bool myComparator(employee i, employee j) { return (i.total < j.total); }
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argc != 3)
		std::cout << "�������� ���������, reporter ��������� ������\n";
	else {
		std::ifstream in(argv[0]);
		std::ofstream fout(argv[1], std::ios_base::trunc | std::ios_base::out);
		if (fout.is_open() && in.is_open()) {
			int salary = std::atoi(argv[2]);
			std::string line;
			std::string buffer;
			employee temp;
			std::vector<employee> employees;
			while (!in.eof()) {
				in >> temp.num >> temp.name >> temp.hours;
				temp.total = temp.hours * salary;
				employees.push_back(temp);
			}
			in.close();
			std::sort(employees.begin(), employees.end(), myComparator);
			fout << "����� �� ����� " << argv[0] << std::endl;
			fout << "����� ����������, ��� ����������, ���������� �����, �������� ��������\n";
			for (employee& var : employees)
				fout << var.num << " " << var.name << " " << var.hours << " " << var.total << std::endl;
			fout.close();
		}
		else
			std::cout << "���� �� ����� ���� ������, reporter ��������� ������\n";
	}
	return 0;
}
