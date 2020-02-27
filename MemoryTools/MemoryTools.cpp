
#include "MemoryTools.h"

////////////////////////////////////////////////////////////////////////////////////////
//List
//		• Listagem de processos em execução no sistema
void LIST::Processes() {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // dwFlagListarProcessos
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL retornoProcess = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (retornoProcess)
		{
			printf("ID: %d  \tNome: %s \n", PE.th32ProcessID, PE.szExeFile);
			retornoProcess = Process32Next(HandleSnapshot, &PE);
		}
	}
	CloseHandle(HandleSnapshot);
}
//		• Listagem de Modulos em execução de um Processo
void LIST::ProcessModules(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // #dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			printf("Nome: %s \t\tBase: %I64X \t\tSize: %d\n", ME.szModule, ME.modBaseAddr, ME.modBaseSize);
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
}
//		• Listagem de Threads em execução de um Processo especificado
void LIST::ProcessThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				printf("Thread ID: %d\n", TE.th32ThreadID);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}
////////////////////////////////////////////////////////////////////////////////////////
//Module
//	Get base address de um Modulo especificado
uintptr_t MODULE::GetBaseAddress(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseAddr;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}
//	Get tamanho de um Modulo especificado
uintptr_t MODULE::GetBaseSize(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseSize;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
//Address
//	Get base address de um Ponteiro especificado passando PID & Handle do Processo
uintptr_t ADRESS::GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets) {
	uintptr_t retornoPointerBaseAddress = 0;
	int tamanho = _Offsets.size();

	uintptr_t MooduleBase = Module::GetBaseAddress(_ProcessID, _MooduleBase);
	retornoPointerBaseAddress = MemoryTools::read.ReadProcess<uintptr_t>(_ProcessHandle, MooduleBase + _Pointer);
	tamanho--;

	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			retornoPointerBaseAddress = MemoryTools::read.ReadProcess<uintptr_t>(_ProcessHandle, retornoPointerBaseAddress + _Offsets[i]);
		}
		else
		{
			retornoPointerBaseAddress = retornoPointerBaseAddress + _Offsets[i];
		}
	}
	return retornoPointerBaseAddress;
}

//	Get base address de um Ponteiro especificado passando apenas PID & Handle do Processo
uintptr_t ADRESS::GetPointerBaseAddress(DWORD _ProcessID, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets) {
	uintptr_t retornoPointerBaseAddress = 0;
	int tamanho = _Offsets.size();
	HANDLE processHandle = MemoryTools::GetProcessHandle(_ProcessID);

	uintptr_t MooduleBase = Module::GetBaseAddress(_ProcessID, _MooduleBase);
	//printf("Module %I64X\n", MooduleBase);
	retornoPointerBaseAddress = MemoryTools::read.ReadProcess<uintptr_t>(processHandle, MooduleBase + _Pointer);
	tamanho--;

	//printf("module + pointer %I64X  \n", retornoPointerBaseAddress);
	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			retornoPointerBaseAddress = MemoryTools::read.ReadProcess<uintptr_t>(processHandle, retornoPointerBaseAddress + _Offsets[i]);
		}
		else
		{
			retornoPointerBaseAddress = retornoPointerBaseAddress + _Offsets[i];
		}
	}
	CloseHandle(processHandle); // Fecha o Handle aberto
	return retornoPointerBaseAddress;
}
////////////////////////////////////////////////////////////////////////////////////////
//Read
class READ {
public:
	template<typename T>
	static T ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) {
		T retornoReadProcess = 0;
		SIZE_T NumberOfBytesToRead = sizeof(retornoReadProcess);
		SIZE_T NumberOfBytesActuallyRead;
		BOOL err = ReadProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), &retornoReadProcess, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
		if (err || NumberOfBytesActuallyRead != NumberOfBytesToRead)
			return retornoReadProcess;
	}

};
////////////////////////////////////////////////////////////////////////////////////////
//Write
class WRITE {
public:
	// Write Process Memory Para Valores Primitivos
	template<typename T>
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, T _buffer) {
		SIZE_T numberOfBytesToRead = sizeof(_buffer);
		return WriteProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), &_buffer, numberOfBytesToRead, NULL);
	}

	// Write Process Memory Para Array of Bytes
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, const std::vector<BYTE>& _buffer) {
		auto bufferToArray = _buffer.data(); // Converte vector para array[]
		const SIZE_T numberOfBytesToRead = _buffer.size();
		return WriteProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), bufferToArray, numberOfBytesToRead, NULL);
	}
};
////////////////////////////////////////////////////////////////////////////////////////
//Thread
enum THREADINFOCLASS {
	ThreadQuerySetWin32StartAddress = 0x09
};

typedef NTSTATUS(WINAPI* ntQueryInformationThread)(
	HANDLE,
	THREADINFOCLASS,
	PVOID,
	ULONG,
	PULONG
	);

ntQueryInformationThread NtQueryInformationThread;

// Suspende a execução das Threads de um Processo especificado
void THREAD::SuspendThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				SuspendThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

// Retoma a execução das Threads de um Processo especificado
void THREAD::ResumeThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				ResumeThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

// Listagem das Start Address das Threads em execução de um Processo especificado
std::string THREAD::GetThreadsStartName(DWORD _ProcessID, uintptr_t _StartAddress)
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID);
	std::string retornoString;
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME);
		while (retornoModule) {
			if ((DWORD)ME.modBaseAddr < _StartAddress && ((uintptr_t)ME.modBaseAddr + (uintptr_t)ME.modBaseSize) > _StartAddress) {
				char formatarString[FILENAME_MAX];
				sprintf(formatarString, "+0x%X", ((uintptr_t)ME.modBaseAddr + ME.modBaseSize) - _StartAddress);
				retornoString = retornoString + ME.szModule;
				retornoString = retornoString + formatarString;
				CloseHandle(HandleSnapshot);
				return retornoString;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return retornoString;
}

void THREAD::ListThreadsStartAddress(DWORD _ProcessID)
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if ((DWORD)NtQueryInformationThread == 0) {
		NtQueryInformationThread = (ntQueryInformationThread)GetProcAddress(GetModuleHandle("Ntdll.dll"), "NtQueryInformationThread");

		if (HandleSnapshot != INVALID_HANDLE_VALUE) {
			THREADENTRY32 TE;
			TE.dwSize = sizeof(TE);
			BOOL retornoThread = Thread32First(HandleSnapshot, &TE);
			while (retornoThread) {
				if (TE.th32OwnerProcessID == _ProcessID) {
					DWORD startAddress = 0;
					HANDLE handleThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, TE.th32ThreadID);
					NtQueryInformationThread(handleThread, ThreadQuerySetWin32StartAddress, &startAddress, sizeof(uintptr_t), nullptr);
					printf("%s \n", GetThreadsStartName(_ProcessID, startAddress).c_str());

					CloseHandle(handleThread);
				}
				retornoThread = Thread32Next(HandleSnapshot, &TE);
			}
		}
	}
	CloseHandle(HandleSnapshot);
}

void THREAD::TerminateThreadsStartAddress(DWORD _ProcessID, const char _NameThread[])
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if ((DWORD)NtQueryInformationThread == 0) {
		NtQueryInformationThread = (ntQueryInformationThread)GetProcAddress(GetModuleHandle("Ntdll.dll"), "NtQueryInformationThread");

		if (HandleSnapshot != INVALID_HANDLE_VALUE) {
			THREADENTRY32 TE;
			TE.dwSize = sizeof(TE);
			BOOL retornoThread = Thread32First(HandleSnapshot, &TE);
			while (retornoThread) {
				if (TE.th32OwnerProcessID == _ProcessID) {
					DWORD startAddress = 0;
					HANDLE handleThread = OpenThread(THREAD_ALL_ACCESS, FALSE, TE.th32ThreadID);
					NtQueryInformationThread(handleThread, ThreadQuerySetWin32StartAddress, &startAddress, sizeof(uintptr_t), nullptr);

					if (strstr(GetThreadsStartName(_ProcessID, startAddress).c_str(), _NameThread) != NULL) {
						TerminateThread(handleThread, 0);
					}
					CloseHandle(handleThread);
				}

				retornoThread = Thread32Next(HandleSnapshot, &TE);
			}
		}
	}
	CloseHandle(HandleSnapshot);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD MemoryTools::GetProcessID(const char _ProcessName[])
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);

		BOOL retornoProcess = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (retornoProcess)
		{
			if (_tcscmp(PE.szExeFile, _ProcessName) == 0) {
				CloseHandle(HandleSnapshot);
				return PE.th32ProcessID;
			}

			retornoProcess = Process32Next(HandleSnapshot, &PE);
		}
	}
	
	CloseHandle(HandleSnapshot);
	return 0;
}

HANDLE MemoryTools::GetProcessHandle(DWORD _ProcessID)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ProcessID); // dwDesiredAccess
}
