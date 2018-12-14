#include <Windows.h>
#include <iostream>
#include <string>
#include <ctype.h>
#include <tlhelp32.h>
using namespace std;

DWORD get_process_id(const char* name)
{
	PROCESSENTRY32 structprocsnapshot = { 0 };
	structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);

	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32First(snapshot, &structprocsnapshot) == FALSE)
		return 0;

	do
	{
		if (strcmp(structprocsnapshot.szExeFile, name) == 0)
		{
			CloseHandle(snapshot);
			return structprocsnapshot.th32ProcessID;
		}
	} while (Process32Next(snapshot, &structprocsnapshot));

	CloseHandle(snapshot);
	return 0;
}

bool inject_dll(const int pid, const char* path)
{
	long dll_size = strlen(path) + 1;
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProc == NULL)
	{
		cerr << "[!]Fail to open target process!" << endl;
		return false;
	}
	cout << "[+]Opening Target Process..." << endl;

	LPVOID MyAlloc = VirtualAllocEx(hProc, NULL, dll_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (MyAlloc == NULL)
	{
		cerr << "[!]Fail to allocate memory in Target Process." << endl;
		return false;
	}

	cout << "[+]Allocating memory in Targer Process." << endl;
	int IsWriteOK = WriteProcessMemory(hProc, MyAlloc, path, dll_size, 0);
	if (IsWriteOK == 0)
	{
		cerr << "[!]Fail to write in Target Process memory." << endl;
		return false;
	}
	cout << "[+]Creating Remote Thread in Target Process" << endl;

	DWORD dWord;
	LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("kernel32"), "LoadLibraryA");
	HANDLE ThreadReturn = CreateRemoteThread(hProc, NULL, 0, addrLoadLibrary, MyAlloc, 0, &dWord);
	if (ThreadReturn == NULL)
	{
		cerr << "[!]Fail to create Remote Thread" << endl;
		return false;
	}

	if ((hProc != NULL) && (MyAlloc != NULL) && (IsWriteOK != ERROR_INVALID_HANDLE) && (ThreadReturn != NULL))
	{
		cout << "[+]DLL Successfully Injected :)" << endl;
		return true;
	}

	return false;
}

int main()
{
	DWORD pid = 0;
	while ((pid = get_process_id("left4dead2.exe")) == 0)
		Sleep(1000);

#ifdef _DEBUG
	auto path = "D:\\Projects\\met0ri_l4d2\\Debug\\met0ri_l4d2_hack.dll";
#else 
	auto path = "D:\\Projects\\met0ri_l4d2\\Release\\met0ri_l4d2_hack.dll";
#endif

	inject_dll(pid, path);
}