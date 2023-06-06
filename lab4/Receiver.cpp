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
    WaitForMultipleObjects(sendersCount, busyEvents, TRUE, INFINITE);//ждем сигнал от всех sender (4 пункт)
    SetEvent(startupEvent); //даем всем им команду на старт
    std::string response;
    while (WaitForMultipleObjects(sendersCount, senders, TRUE, 0) == WAIT_TIMEOUT) {//пока все sender не завершат работу
        std::cout << "Введите 0 для попытки чтения сообщений из файла.\n";
        do {
            std::cin >> response;
        } while (response.compare("0") != 0);
        std::string message;
        fin.sync();//синхронизируем поток чтения и файл (могло добавиться сообщений), 
        //я уже хз зачем конкретно нужно, вроде бы, чтобы указатель норм работал, не двигался
        if (!(std::getline(fin, message))) {//если не удалось прочитать
            std::cout << "Новые сообщения отсутствуют. Ожидание.\n";
            fin.clear();//см строку 49, а еще лучше документацию
            ResetEvent(startupEvent); //мы тут ждем новые сообщения, следовательно, нужно, чтоюы sender знал,
            //что нужно уведомить reciever, что появилось новое сообщение. А саму информацию о появлении 
            //reciever получает через busyEvents в строке ниже
            do {
                if (WaitForMultipleObjects(sendersCount, busyEvents, FALSE, 5000) != WAIT_TIMEOUT)
                    //в течение 5000мс ждем появление новых сообщений
                    break;
            } while (WaitForMultipleObjects(sendersCount, senders, TRUE, 0) == WAIT_TIMEOUT);//если не появилось, проверяем, активны ли вообще senders
            SetEvent(startupEvent);
        }
        else {
            std::cout << message << std::endl; //если удалось прочитать, выводим сообщение
        }
        while (std::getline(fin, message)) {
            std::cout << message << std::endl;
        }//выводим все оставшиеся до конца файла новые записи (в ифе мы сначала проверили, 
        //есть ли они вообще и прочитали первое). Если их нет, этот цикл выполняться не буудет
        fin.clear();//см строку 49, а еще лучше документацию
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
