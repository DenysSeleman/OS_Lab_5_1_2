#include <iostream>
#include<Shlwapi.h>
#include <tchar.h>
#pragma comment(lib,"Shlwapi")
using namespace std;
const int dataSize = 1000;
int main()
{
	HANDLE hProc = NULL;
	DWORD procID = 0;
	cout << "Enter process ID: ";
	cin >> procID;
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	if (!hProc)
	{
		cerr << "Can not open this process: error " << GetLastError() << "\n";
		return 1;
	}
	else
		cout << "Process has been found and is ready to reading.\n";

	CHAR buffer[dataSize], bufferPrev[dataSize];
	SIZE_T bytesRead;
	wchar_t addrStr[100];
	LONGLONG addr;
	cout << "Enter data address: ";
	wcin >> addrStr;
	StrToInt64Ex(addrStr, STIF_SUPPORT_HEX, &addr);

	if (ReadProcessMemory(hProc, (LPCVOID)addr, (LPVOID)buffer, dataSize, &bytesRead))
		_tprintf(_T("The data has been read successfully: %hs\n"), buffer);
	else
	{
		cerr << "Can not read the data: error " << GetLastError() << "\n";
		return 2;
	}

	cout << "\nIf the data changes, it will be displayed here.\n";
	strcpy_s(bufferPrev, buffer);
	while (ReadProcessMemory(hProc, (LPCVOID)addr, (LPVOID)buffer, dataSize, &bytesRead))
	{
		if (strcmp(buffer, bufferPrev))
		{
			_tprintf(_T("The data has been changed: %hs\n"), buffer);
			strcpy_s(bufferPrev, buffer);
		}
		Sleep(100);
	}

	cout << "Can not read the data: error " << GetLastError() << "\n";
	CloseHandle(hProc);
	return 0;
}