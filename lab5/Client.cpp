#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>


int main(int argc, char* argv[])
{
    if (argc == 2) {
        setlocale(LC_ALL, "Russian");
        int mode = 1;
        int bufferSize = sizeof(char) * atoi(argv[1]);
        bool isSuccessful;
        char* response = new char[atoi(argv[1])];
        std::string toPipe;
        while (mode) {
            std::cout << "Введите:\n0 для прекращения работы;\n1 для чтения записи о сотруднике;\n2 для редактирования записи о сотруднике;\n";
            std::cin >> mode;
            switch (mode){
            case 1:
                //done
                std::cout << "Введите идентификационный номер сотрудника или 0 для возврата назад.\n";
                std::cin >> mode;
                if (mode == 0) {
                    mode++;
                    continue;
                }
                else {
                    toPipe = "1" + std::to_string(mode);
                    int bytesRead;
                    isSuccessful = CallNamedPipeA(argv[0], (LPVOID)toPipe.c_str(), bufferSize,
                        response, bufferSize, (LPDWORD)&bytesRead, NMPWAIT_WAIT_FOREVER);
                    if (response[0] == '1') {
                        char* next_token = NULL;
                        char* word = strtok_s(response, " ", &next_token);
                        word = strtok_s(NULL, " ", &next_token);
                        std::cout << "Имя сотрудника: " << word << std::endl;
                        word = strtok_s(NULL, " ", &next_token);
                        std::cout << "Количество отработанных часов: " << word << std::endl;
                    }
                    else {
                        if (response[0] == '0') {
                            std::cout << "Сотрудника с таким идентификационным номером не существует.\n";
                        }
                        else {
                            std::cout << "В данный момент запись обрабатывается, в доступе отказано.\n";

                        }
                    }
                }
                break;
            case 2:
                //not done
                std::cout << "Введите идентификационный номер сотрудника или 0 для возврата назад.\n";
                std::cin >> mode;
                if (mode == 0) {
                    mode++;
                    continue;
                }
                else {
                    toPipe = "2" + std::to_string(mode);
                    int bytesRead;
                    isSuccessful = CallNamedPipeA(argv[0], (LPVOID)toPipe.c_str(), bufferSize,
                        response, bufferSize, (LPDWORD)&bytesRead, NMPWAIT_WAIT_FOREVER);
                    if (response[0] == '1') {
                        char* next_token = NULL;
                        char* word = strtok_s(response, " ", &next_token);
                        word = strtok_s(NULL, " ", &next_token);
                        std::cout << "Имя сотрудника: " << word << std::endl;
                        word = strtok_s(NULL, " ", &next_token);
                        std::cout << "Количество отработанных часов: " << word << std::endl;
                        std::cout << "Введите новое имя сотрудника.\n";
                        std::string newName;
                        std::cin >> newName;
                        std::cout << "Введите новое количество отработанных сотрудником часов.\n";
                        double newHours;
                        std::cin >> newHours;
                        toPipe = "0" + std::to_string(mode) + " " + newName + " " + std::to_string(newHours);
                        isSuccessful = CallNamedPipeA(argv[0], (LPVOID)toPipe.c_str(), bufferSize,
                            response, bufferSize, (LPDWORD)&bytesRead, NMPWAIT_WAIT_FOREVER);
                        if(response[0] == '1')
                            std::cout << "Запись изменена.\n";
                        else
                            std::cout << "Запись не была изменена, код: " << response[0] << std::endl;
                    }
                    else {
                        if (response[0] == '0') {
                            std::cout << "Сотрудника с таким идентификационным номером не существует.\n";
                        }
                        else {
                            std::cout << "В доступе отказано, код: " << response[0] << std::endl;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    //std::cout << argc;
    return 0;
}
