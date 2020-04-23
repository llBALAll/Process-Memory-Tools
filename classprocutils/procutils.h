
#include "pch.h"


class PRINT {
public:
	static void ProcList();
	static void PIDsByName(char* procName);
	static void ModList(DWORD procPID);
	static int ModListOrder(DWORD procPID);// revisar
	static void ThreadsList(DWORD procPID);
	static void ThrdsStartAddr(DWORD procPID);
	static void ThrdsStartAddrOff(DWORD procPID);
};

class GET {
public:
	static DWORD ProcPID(char* procName);
	static DWORD* ProcPIDsRet(char* procName);
	static int ProcPIDs(char* procName, DWORD* PprocPID);
	static BOOL ProcName(DWORD procPID, char* procName);
	static std::string ThrdStartNameByAddr(DWORD procPID, uintptr_t StartAddress);
	static int ProcTIDs(DWORD procPID, DWORD* PprocTID);
	static int ProcModules(DWORD procPID, char buffProcMods[][MAX_MODULE_NAME32]);
	static DWORD ModBaseAddr(DWORD procPID, char* moduleName);
	static uintptr_t ModBaseAddr(DWORD procPID, wchar_t* moduleName);
	static DWORD ModBaseSize(DWORD procPID, wchar_t* moduleName);
};

class TOOL {
public:
	static BOOL PauseThreads(DWORD procPID);
	static BOOL ResumeThreads(DWORD procPID);
	static void PauseAndResumeThreads(DWORD procPID);
	static void ProcMonitor(char* procName);
	static void printHotkeyInfo(void);
	static void HotkeyLoop(DWORD procPID);
	static BOOL InjectDll(DWORD procPID, char* dllpath);
	static void correctPath(char* pathIn);
	static BOOL killProcessByName (char *procName)
	static void ProcessKiller (char* procName);
};

class MEMORY {
public:
	static BOOL ReadProcMem_INT(DWORD procPID, uintptr_t procAddr, int& rData);
	static BOOL WriteProcMem_INT(DWORD procPID, uintptr_t procAddr, int& wData);
};


class ProcUtils {
public:
	static PRINT Print;
	static GET Get;
	static TOOL Tool;
	static MEMORY Memory;
};
