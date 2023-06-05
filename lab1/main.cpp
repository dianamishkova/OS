#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите название исходного файла\n";
    std::string sourceFileName;
    std::cin >> sourceFileName;
    std::cout << "Введите количество записей\n";
    int notesCount;
    std::cin >> notesCount;
    STARTUPINFOA creatorSI;
    PROCESS_INFORMATION creatorPI;
    ZeroMemory(&creatorSI, sizeof(creatorSI)); 
    creatorSI.cb = sizeof(creatorSI);           
    ZeroMemory(&creatorPI, sizeof(creatorPI));  
    std::string arguments = sourceFileName + " " + std::to_string(notesCount); 
    const char* CreatorAppName = "Creator.exe";
    if (CreateProcessA(CreatorAppName, (char*)arguments.c_str(),
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &creatorSI, &creatorPI)) {
        WaitForSingleObject(creatorPI.hProcess, INFINITE);
        CloseHandle(creatorPI.hThread);
        CloseHandle(creatorPI.hProcess);
        std::fstream fin(sourceFileName, std::ios_base::in | std::ios_base::binary);
        char a;                         
        while (fin.read(&a, sizeof(a))) 
            std::cout << a;             
        fin.close();
        std::cout << "Введите название файла отчета\n";
        std::string fileReportName;
        std::cin >> fileReportName;
        std::cout << "Введите почасовую ставку\n";
        int salary;
        std::cin >> salary;
        STARTUPINFOA reporterSI;
        PROCESS_INFORMATION reporterPI;
        ZeroMemory(&reporterSI, sizeof(reporterSI));
        reporterSI.cb = sizeof(reporterSI);
        ZeroMemory(&reporterPI, sizeof(reporterPI));
        const char* ReporterAppName = "Reporter.exe";
        arguments = sourceFileName + " " + fileReportName + " " + std::to_string(salary);
        if (CreateProcessA(ReporterAppName, (char*)arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &reporterSI, &reporterPI)) {
            WaitForSingleObject(reporterPI.hProcess, INFINITE);
            CloseHandle(reporterPI.hThread);
            CloseHandle(reporterPI.hProcess);
            std::fstream fin(fileReportName, std::ios_base::in | std::ios_base::binary);
            char a;
            while (fin.read(&a, sizeof(a)))
                std::cout << a;
            fin.close();
        }
        else
        {
            std::cout << "Не удалось создать процесс Creator\n" << GetLastError();
        }
    }
    else
    {
        std::cout << "Не удалось создать процесс Reporter\n" << GetLastError();
    }
    return 0;
}