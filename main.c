/*
 *	создаются 10 потоков
 *	каждый увеличивает глобальный счетчик на 1 по 100 раз.
 *	монопольный доступ к счетчику регулируется с помощью мьютекса.
 *
 *	ВСЕ ДЕСКРИПТОРЫ НУЖНО ЗА СОБОЙ ЗАКРЫВАТЬ
 *
 *	? строки обернуты в макрос TEXT ?
 *
 *	
 * */

#include <windows.h>
#define THREADS_NUMBER 10
#define ITERATIONS_NUMBER 100
#define PAUSE 10 /*ms*/

DWORD dwCounter = 0;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
  CONST HANDLE hMutex = (CONST HANDLE)lpParam;
  DWORD i;
  for (i = 0; i < ITERATIONS_NUMBER; i++) {
  	WaitForSingleObject(hMutex, INFINITE);
	ReleaseMutex(hMutex);
	Sleep(PAUSE);	
  }
  ExitThread(0);
} 

VOID Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage)
{
    DWORD dwTemp;
    TCHAR szError[256];
    WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);
    wsprintf(szError, TEXT("LastError=%d\r\n"), GetLastError());
    WriteConsole(hStdOut, szError, lstrlen(szError), &dwTemp, NULL);
    ExitProcess(0);
}

INT main() {
    TCHAR szMessage[256];
    DWORD dwTemp, i;
    HANDLE hThreads[THREADS_NUMBER];
    CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONST HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
    if (NULL == hMutex) {
        Error(hStdOut, TEXT("Failed to create mutex.\r\n"));
    }

    for (i = 0; i < THREADS_NUMBER; i++) {
        hThreads[i] = CreateThread(NULL, 0, &ThreadProc, hMutex, 0, NULL);
       if (NULL == hThreads[i]) {
	       Error(hStdOut, TEXT("Failed to create thread.\t\n"));
       }	

    }

    WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);
    wsprintf(szMessage, TEXT("Counter = %d\r\n"), dwCounter);
    WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);

    for (i = 0; i < THREADS_NUMBER; i++) {
    	CloseHandle(hThreads[i]);
    }
    CloseHandle(hMutex);
    ExitProcess(0);
}
