#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <charconv>

int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите название исходного файла\n";
    std::string fileName;
    std::cin >> fileName;
    std::cout << "Введите количество процессов Sender\n";
    int sendersCount;
    do {
        std::cin >> sendersCount;
    } while (sendersCount <= 0);
    STARTUPINFOA* sendersSI = new STARTUPINFOA[sendersCount];
    PROCESS_INFORMATION* sendersPI = new PROCESS_INFORMATION[sendersCount];
    HANDLE* busyEvents = new HANDLE[sendersCount];
    HANDLE* senders = new HANDLE[sendersCount];
    std::fstream fin(fileName, std::ios_base::trunc | std::ios_base::out | std::ios_base::binary | std::ios_base::in);
    const char* appName = "C:\\Users\\usver\\source\\repos\\OS4\\Sender\\Debug\\Sender.exe";
    SECURITY_ATTRIBUTES securityAttributes = {sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE startupEvent = CreateEventA(&securityAttributes, TRUE, FALSE, "SenderStartupEvent");
    HANDLE mutex = CreateMutexA(&securityAttributes, FALSE, "SenderWriteMutex");
    for (int i = 0; i < sendersCount; i++) {
        ZeroMemory(&sendersSI[i], sizeof(STARTUPINFO));
        sendersSI[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&sendersPI[i], sizeof(PROCESS_INFORMATION));
        busyEvents[i] = CreateEventA(&securityAttributes, FALSE, FALSE, ("busyEvent" + std::to_string(i)).c_str());
        std::string arguments = fileName + " busyEvent" + std::to_string(i);
        if (!CreateProcessA(appName, (char*)arguments.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &sendersSI[i], &sendersPI[i])) {
            std::cout << GetLastError();
            ExitProcess(0);
        }
        senders[i] = sendersPI[i].hProcess;
    }//выполнили первые три пункта
    WaitForMultipleObjects(sendersCount, busyEvents, TRUE, INFINITE);
    SetEvent(startupEvent); //даем всем им команду на старт
    std::string response;
    while (WaitForMultipleObjects(sendersCount, senders, TRUE, 0) == WAIT_TIMEOUT) {
        std::cout << "Введите 0 для попытки чтения сообщений из файла.\n";
        do {
            std::cin >> response;
        } while (response.compare("0") != 0);
        std::string message;
        fin.sync();
        if (!(std::getline(fin, message))) {
            std::cout << "Новые сообщения отсутствуют. Ожидание.\n";
            fin.clear();
            ResetEvent(startupEvent); 
            
            do {
                if (WaitForMultipleObjects(sendersCount, busyEvents, FALSE, 5000) != WAIT_TIMEOUT)
                    
                    break;
            } while (WaitForMultipleObjects(sendersCount, senders, TRUE, 0) == WAIT_TIMEOUT);
            SetEvent(startupEvent);
        }
        else {
            std::cout << message << std::endl; 
        }
        while (std::getline(fin, message)) {
            std::cout << message << std::endl;
        }
        fin.clear();
    }
    for (int i = 0; i < sendersCount; i++) {
        CloseHandle(sendersPI[i].hThread);
        CloseHandle(sendersPI[i].hProcess);
        CloseHandle(busyEvents[i]);
    }
    CloseHandle(startupEvent);
    CloseHandle(mutex);
    fin.close();
    return 0;
}
