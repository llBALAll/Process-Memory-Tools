#pragma once

#include "pch.h"


class LIST {
public:
	static void Processes();
	static void ProcessModules(DWORD _ProcessID);
	static void ProcessThreads(DWORD _ProcessID);
};

class MODULE {
public:
	static uintptr_t GetBaseAddress(DWORD _ProcessID, const char _ModuleName[]);
	static uintptr_t GetBaseSize(DWORD _ProcessID, const char _ModuleName[]);
};

class Address {
public:
	static uintptr_t GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets);
	static uintptr_t GetPointerBaseAddress(DWORD _ProcessID, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets);
};

class READ {
public:
	template<typename T>
	static T ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress);
};

class WRITE {
public:
	// Write Process Memory Para Valores Primitivos
	template<typename T>
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, T _buffer) 

	// Write Process Memory Para Array of Bytes
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, const std::vector<BYTE>& _buffer);
};

class THREAD {
public:
	static void SuspendThreads(DWORD _ProcessID);
	static void ResumeThreads(DWORD _ProcessID);
	static std::string GetThreadsStartName(DWORD _ProcessID, uintptr_t _StartAddress);
	static void ListThreadsStartAddress(DWORD _ProcessID);
	static void TerminateThreadsStartAddress(DWORD _ProcessID, const char _NameThread[]);
};


class MemoryTools {
public:
	static DWORD GetProcessID(const char _ProcessName[]);
	static HANDLE GetProcessHandle(DWORD _ProcessID);

	static LIST List;
	static MODULE Module;
	static ADRESS Address;
	static READ Read;
	static WRITE Write;
	static THREAD Thread;
};
