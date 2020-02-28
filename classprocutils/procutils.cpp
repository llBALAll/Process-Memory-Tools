
#include "pch.h"


// Imprime uma lista com nome e PID dos processos em execucao;
void PRINT::ProcList () {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnap, &PE);
		if (Retorno) printf("	ID	Processo\n\n");
		while (Retorno) {
			printf("	%lu	%s\n", PE.th32ProcessID, PE.szExeFile);
			//printf("ID: %d		Processo: %S\n", PE.th32ProcessID, PE.szExeFile);
			//std::wcout << "ID: " << PE.th32ProcessID << "    Nome: " << PE.szExeFile << std::endl;
			Retorno = Process32Next(HandleSnap, &PE);
		}
	}
	CloseHandle(HandleSnap);
}

// Busca pelo nome do processo e imprime todos os PIDs;
void PRINT::PIDsByName (char* procName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnap, &PE);
		while (Retorno) {
			if (strcmp(PE.szExeFile, procName) == 0) {
				//printf("  %s    %d\n", PE.szExeFile, PE.th32ProcessID);
				printf("\n\t%lu\t%s", PE.th32ProcessID, PE.szExeFile);
				//printf("  ID: %d	Nome: %s\n", PE.th32ProcessID, PE.szExeFile);
				//std::cout << "ID: " << PE.th32ProcessID << "    Nome: " << PE.szExeFile << std::endl;
			}
			Retorno = Process32Next(HandleSnap, &PE);
		}
	}
	CloseHandle(HandleSnap);
}

// Busca pelo nome do processo e Retorna o primeiro PID encontrado;
DWORD GET::ProcPID (char* procName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnap, &PE);
		while (Retorno) {
			if (strcmp(PE.szExeFile, procName) == 0) {
				CloseHandle(HandleSnap);
				return PE.th32ProcessID;
			}
			Retorno = Process32Next(HandleSnap, &PE);
		}
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Busca pelo nome do processo e Retorna o primeiro PID encontrado;
DWORD GET::ProcPIDW (wchar_t* procName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32FirstW(HandleSnap, &PE);
		while (Retorno) {
			if (wcscmp((LPWSTR)PE.szExeFile, procName) == 0) {
				CloseHandle(HandleSnap);
				return PE.th32ProcessID;
			}
			Retorno = Process32NextW(HandleSnap, &PE);
		}
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Busca pelo nome do processo e Retorna um endereco do primeiro elemento de um array de PIDs encontrados com o mesmo nome de processo;
// Serve para o caso de haver varios processos com mesmo nome
DWORD* GET::ProcPIDsRet (char* procName) {
	int i = 0;
	int max_process = 100;
	DWORD* PprocPID;

	MALLOC(PprocPID, max_process * sizeof(DWORD));
	memset(PprocPID, 0, max_process * sizeof(DWORD));

	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnap, &PE);
		while (Retorno) {
			if (strcmp(PE.szExeFile, procName) == 0) {
				PprocPID[i] = PE.th32ProcessID;
				i++;
			}
			Retorno = Process32Next(HandleSnap, &PE);
		}
		PprocPID[i] = '\0';
		CloseHandle(HandleSnap);
		return PprocPID;
	}
	CloseHandle(HandleSnap);
	free(PprocPID);
	return 0;
}

// Recebe um nome de processo e Busca pelo nome do processo Retornando a quantidade de PIDs encontrados e
// um paramentro de retorno contendo um ponteiro para um array de PIDs obtidos na busca;
int GET::ProcPIDs (char* procName, DWORD* PprocPID) {
	int i = 0;
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnap, &PE);
		while (Retorno) {
			if (strcmp(PE.szExeFile, procName) == 0) {
				*(PprocPID + i) = PE.th32ProcessID;
				i++;
			}
			Retorno = Process32Next(HandleSnap, &PE);
		}
		*(PprocPID + i) = '\0';
		CloseHandle(HandleSnap);
		return i;
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Recebe o Process ID(PID) e busca pelo process name passando como parametro um ponteiro para um array de char
// retornando true ou false de acordo com a busca;
BOOL GET::ProcName (DWORD procPID, char* procName) {
	MODULEENTRY32 ME = { 0 };
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);

	if(!HandleSnap) return FALSE;
	ME.dwSize = sizeof(ME);
	BOOL bModule = Module32First(HandleSnap, &ME);
	if(bModule) {
		//strncpy(procName, (const char*) ME.szModule, MAX_PATH-1);
		strncpy_s(procName, MAX_PATH-1, (const char*) ME.szModule,MAX_PATH-1);
		//cloneArraysChar(procName, ME.szModule);
		CloseHandle(HandleSnap);
		return EXIT_SUCCESS;
	}
	else {
		CloseHandle(HandleSnap);
		return EXIT_FAILURE;
	}
}

// A partir do PID do processo Imprime uma lista com o ID das threads do processo;
void PRINT::ThreadsList (DWORD procPID) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
		//if (Retorno) printf("\nThreads of Process ID: %d\n", procPID);
		//if (Retorno) printf("\n\tThreads:");
		while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				//printf("\tThread ID: %lu\n", TE.th32ThreadID);
				printf("\n\t\t%lu", TE.th32ThreadID);
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
	}
	CloseHandle(HandleSnap);
}

enum THREADINFOCLASS {
	ThreadQuerySetWin32StartAddress = 0x09
};

typedef long NTSTATUS; // se for compilar para 32bits

typedef NTSTATUS (WINAPI *_NtQueryInformationThread)(
	HANDLE,
	THREADINFOCLASS,
	PVOID,
	ULONG,
	PULONG
);
_NtQueryInformationThread NtQueryInformationThread;

// A partir do PID do processo Imprime uma lista com o ID das threads do processo;
void PRINT::ThrdsStartAddr(DWORD procPID) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (NtQueryInformationThread == 0)
		NtQueryInformationThread = (_NtQueryInformationThread)GetProcAddress(GetModuleHandleA("Ntdll.dll"), "NtQueryInformationThread");
	if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
		while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				DWORD StartAddress = 0;
				HANDLE THAccess = OpenThread(THREAD_QUERY_INFORMATION, FALSE, TE.th32ThreadID);
				NtQueryInformationThread(THAccess, ThreadQuerySetWin32StartAddress, &StartAddress, sizeof(uintptr_t), nullptr);
				printf("\n\t\t%lu\t\t%X", TE.th32ThreadID, StartAddress);
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
	}
	CloseHandle(HandleSnap);
}

std::string GET::ThrdStartNameByAddr(DWORD procPID, uintptr_t StartAddress) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procPID);
	std::string retornoString;
	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnap, &ME);
		while (retornoModule) {
			if ((StartAddress > (DWORD)ME.modBaseAddr) && (StartAddress < ((uintptr_t)ME.modBaseAddr + (uintptr_t)ME.modBaseSize))) {
				char formatarString[FILENAME_MAX];
				//sprintf(formatarString, "+0x%X", ((uintptr_t)ME.modBaseAddr + ME.modBaseSize) - _StartAddress);
				sprintf_s(formatarString, " + 0x%X", (StartAddress - (uintptr_t)ME.modBaseAddr));
				//printf("\nStartAddress: 0x%X", StartAddress);
				//printf("\nME.modBaseAddr: 0x%X", ME.modBaseAddr);
				//printf("\nME.modBaseSize: 0x%X", ME.modBaseSize);
				//printf("\nStartAddress - ME.modBaseAddr: 0x%X\n", StartAddress - (uintptr_t)ME.modBaseAddr);
				retornoString = retornoString + ME.szModule;
				retornoString = retornoString + formatarString;
				CloseHandle(HandleSnap);
				return retornoString;
			}
			retornoModule = Module32Next(HandleSnap, &ME);
		}
	}
	CloseHandle(HandleSnap);
	return retornoString;
}

void PRINT::ThrdsStartAddrOff(DWORD procPID) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (NtQueryInformationThread == 0)
		NtQueryInformationThread = (_NtQueryInformationThread)GetProcAddress(GetModuleHandleA("Ntdll.dll"), "NtQueryInformationThread");
	if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
		printf("\n\tTID\tModule + Offset\n", TE.th32ThreadID);
		while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				DWORD StartAddress = 0;
				HANDLE THAccess = OpenThread(THREAD_QUERY_INFORMATION, FALSE, TE.th32ThreadID);
				NtQueryInformationThread(THAccess, ThreadQuerySetWin32StartAddress, &StartAddress, sizeof(uintptr_t), nullptr);
				//printf("\n  TID: %lu\t\t", TE.th32ThreadID);
				printf("\n\t%lu\t", TE.th32ThreadID);
				printf("%s ", ProcUtils::Get.ThrdStartNameByAddr(procPID, StartAddress).c_str());
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
	}
	CloseHandle(HandleSnap);
}

// Recebe um PID e Busca pelas threads do PID retornando a quantidade de threads encontradas e
// um paramentro de retorno contendo um ponteiro para um array de Threads IDs obtidos na busca;
int GET::ProcTIDs (DWORD procPID, DWORD* PprocTID) {
	int i = 0;
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
		while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				*(PprocTID + i) = TE.th32ThreadID;
				i++;
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
		*(PprocTID + i) = '\0';
		CloseHandle(HandleSnap);
		return i;
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Recebe um process ID e Pausa todas threads deste PID
BOOL TOOL::PauseThreads (DWORD procPID) {
    BOOL flagReturn = EXIT_SUCCESS;

    HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
        while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				HANDLE THAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID);
				if (SuspendThread(THAccess) == (DWORD) -1){
                    printf("Can't pause thread ID: %lu\n", TE.th32ThreadID);
                    flagReturn = EXIT_FAILURE;
				}
				else {
                    //printf("Paused thread ID: %d\n", TE.th32ThreadID);
				}
				CloseHandle(THAccess);
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
    }
    else {
        flagReturn = EXIT_FAILURE;
    }
    CloseHandle(HandleSnap);
    return flagReturn;
}

// Recebe um process ID e Resume as todas as threads deste PID
BOOL TOOL::ResumeThreads (DWORD procPID) {
    BOOL flagReturn = EXIT_SUCCESS;

    HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (HandleSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnap, &TE);
        while (Retorno) {
			if (TE.th32OwnerProcessID == procPID) {
				HANDLE THAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID);
				if (ResumeThread(THAccess) == (DWORD) -1){
                    printf("Can't resume thread ID: %lu\n", TE.th32ThreadID);
                    flagReturn = EXIT_FAILURE;
				}
				else {
                    //printf("Resumed thread ID: %d\n", TE.th32ThreadID);
				}
				CloseHandle(THAccess);
			}
			Retorno = Thread32Next(HandleSnap, &TE);
		}
    }
    else {
        flagReturn = EXIT_FAILURE;
    }
    CloseHandle(HandleSnap);
    return flagReturn;
}

// Procedimento para pausar e resumir todas as threads de um process ID
void TOOL::PauseAndResumeThreads(DWORD procPID) {
   	char conf;

	printf ("\nWant to pause all threads of process?(y/n) ");
	//scanf("%c", &conf);
	std::cin >> conf;
	if (conf == 'y') {
        if (PauseThreads(procPID) == EXIT_SUCCESS) printf ("Sucess pausing all threads of PID: %lu\n", procPID);
        else printf ("Fail on suspend of any thread of process ID (%lu)\n", procPID);

        printf("Press enter to resume all threads of Process ID: %lu", procPID);
        fflush(stdin);
        getchar();
        if (ResumeThreads(procPID) == EXIT_SUCCESS) printf ("Sucess resuming all threads of PID: (%lu)\n", procPID);
        else printf ("Fail on resume of any thread of process ID (%lu)\n", procPID);
    }
}

// A partir do PID do processo Imprime uma lista com nome, base addr e size de cada modulo do processo;
void PRINT::ModList (DWORD procPID) {
	int modCount = 0;
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnap, &ME);
		//if (Retorno) printf("\n  Modulo\t\tBaseAddr\t\tSize\n");
		if (Retorno) printf("\n  Modulo\t\tBaseAddr\t\t\tSize\n");
		while (Retorno) {
			//printf("  %s\n\t\t\t0x%p\t%lu\n", ME.szModule, ME.modBaseAddr, ME.modBaseSize);
			printf("\n  %s\t\t0x%p\t\t%lu", ME.szModule, ME.modBaseAddr, ME.modBaseSize);
			Retorno = Module32Next(HandleSnap, &ME);
			modCount++;
		}
		CloseHandle(HandleSnap);
	}
	printf("\n\n  Total: %d", modCount);
	CloseHandle(HandleSnap);
}

// A partir do PID do processo Imprime em ordem alfabetica uma lista com nome, (falta fazer) base addr e size de cada modulo do processo;
// Retorna o total de modulos do process ID;
int PRINT::ModListOrder (DWORD procPID) {
	int i=0,j=0,c=0;
	char buffProcMods [MAX_PATH][MAX_MODULE_NAME32];
	char sAux[MAX_MODULE_NAME32];

	if ((c=ProcUtils::Get.ProcModules (procPID, buffProcMods)) == 0) {
		printf ("Can not find modules!");
		return 0;
	}

	// converte as strings para minuscula
	for (i = 0;i < c; i++) {
		//StrLowerSelf(buffProcMods[i]);
		_strlwr_s(buffProcMods[i]);
	}

	// coloca as strings em ordem alfabetica
	for(i=0;i < c; i++) {
		for(j=0;j < c; j++) {
			if(strcmp(buffProcMods[i], buffProcMods[j]) < 0) {
				strcpy_s(sAux, buffProcMods[i]);
				strcpy_s(buffProcMods[i], buffProcMods[j]);
				strcpy_s(buffProcMods[j], sAux);
			}
		}
	}

	printf("\n  Modulo\n");
	for (i=0;i < c;i++) {
		printf ("\n  %s", buffProcMods[i]);
	}
	printf ("\n\n  Total: %d", i);

	return c;
}

// Recebe um process ID e Busca pelos modulos do PID retornando a quantidade de modulos encontrados e
// um paramentro de retorno contendo um ponteiro para um array de modulos obtidos na busca;
int GET::ProcModules (DWORD procPID, char buffProcMods [][MAX_MODULE_NAME32]) {
	int i = 0;
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnap, &ME);
		while (Retorno) {
			//strncpy(buffProcMods[i], (const char*) ME.szModule, MAX_PATH-1);
			strncpy_s(buffProcMods[i], MAX_PATH-1, (const char*) ME.szModule,MAX_PATH-1);
			Retorno = Module32Next(HandleSnap, &ME);
			i++;
		}
		//PPbuffProcMods[i] = "\0";////// comentado com (char buffProcMods [] [MAX_MODULE_NAME32])
		CloseHandle(HandleSnap);
		return i;
	}
	CloseHandle(HandleSnap);
	return 0;
}

DWORD GET::ModBaseAddr (DWORD procPID, char* moduleName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);
	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnap, &ME);
		while (Retorno) {
			if (strcmp(ME.szModule, moduleName) == 0) {
				CloseHandle(HandleSnap);
				//printf("\n  BaseAddr: 0x%p\n", ME.modBaseAddr);
				return (DWORD)ME.modBaseAddr;
			}
			Retorno = Module32Next(HandleSnap, &ME);
		}
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Busca por um modulo em um processo dado o seu PID, e Retorna o endereco base que foi carregado o modulo especifico;
// Util quando se quer saber o endereco base de uma dll carregada em um executavel em tempo de execucao;
uintptr_t GET::ModBaseAddr (DWORD procPID, wchar_t* moduleName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnap, &ME);
		while (Retorno) {
			if (wcscmp((LPWSTR)ME.szModule, moduleName) == 0) {
				CloseHandle(HandleSnap);
				//printf("\n  BaseAddr: 0x%p\n", ME.modBaseAddr);
				return (uintptr_t) ME.modBaseAddr;
			}
			Retorno = Module32Next(HandleSnap, &ME);
		}
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Busca por um modulo em um processo dado o seu PID, e Retorna o tamanho do modulo especifico em bytes;
// Util quando se quer saber o tamanho de uma dll carregada no executavel em tempo de execucao;
DWORD GET::ModBaseSize (DWORD procPID, wchar_t* moduleName) {
	HANDLE HandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procPID);

	if (HandleSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnap, &ME);
		while (Retorno) {
			if (wcscmp((LPWSTR)ME.szModule, moduleName) == 0) {
				CloseHandle(HandleSnap);
				//printf("\n  BaseAddr: 0x%p\n", ME.modBaseSize);
				return ME.modBaseSize;
			}
			Retorno = Module32Next(HandleSnap, &ME);
		}
	}
	CloseHandle(HandleSnap);
	return 0;
}

// Loop que monitora todas os processos com um mesmo process name, como tambem todas as threads de todos esses processos;
// Atualizando/Imprimindo na tela a cada nova instancia de processo/thread quem tenha entrado em execucao;
void TOOL::ProcMonitor (char* procName) {

	BOOL flagPrintWait = FALSE;

	int i;
	int cPIDs = 0;
	int cPIDsMem = 1000;
	BOOL cPIDsChange = FALSE;			// flag para detectar mudanca no total de PIDs (cPIDs)
	DWORD buffPIDs [MAX_PIDs];

	int j;
	int cTIDs [MAX_PIDs];
	int cTIDsMem [MAX_PIDs];
	BOOL cTIDsChange = FALSE;			// flag para detectar mudanca no total de TIDs para cada PID
	DWORD mTIDs [MAX_PIDs] [MAX_TIDs];	// matrix de Threads IDs (TIDs), onde cada linha contem todos os TIDs do PID correspondente
		
	while (1) {

		cPIDsChange = FALSE;
		cPIDs = ProcUtils::Get.ProcPIDs (procName, buffPIDs);
		if (cPIDs != cPIDsMem) {
			cPIDsMem = cPIDs;
			cPIDsChange = TRUE;
		}

		cTIDsChange = FALSE;
		for (i=0; i < cPIDs ;i++) {
			cTIDs[i] = ProcUtils::Get.ProcTIDs (buffPIDs[i], &mTIDs[i][0]);
			if (cTIDs[i] != cTIDsMem[i]) {
				cTIDsMem[i] = cTIDs[i];
				cTIDsChange = TRUE;
			}
		}

		if ((cPIDsChange) || (cTIDsChange)) {		//verifica se a contagem de PIDs(cPIDs) para um mesmo nome de processo eh igual a contagem anterior(cPIDsMem)
			system("CLS");
			printf("\nProcess: %s\n", procName);
			printf("\n\tPID\tTID\n");
			//PrintPIDsByName(procName);
			for (i=0; i < cPIDs ;i++) {
				printf("\n\t%lu\t", buffPIDs[i]);
				//PrintThreadsList(buffPIDs[i]);
				//cTIDs[i] = GetProcTIDs (buffPIDs[i], &mTIDs[i][0]);
				for (j=0; j < cTIDs[i] ;j++) {
					printf("\n\t\t%lu", mTIDs[i][j]);
				}
				printf ("\n\t\tTIDs: %d", cTIDs[i]);
			}
			printf ("\n\tPIDs: %d", cPIDs);

			flagPrintWait = TRUE;
		}
		else {
			if (flagPrintWait == TRUE) {
				printf("\nWaiting process ...");
				flagPrintWait = FALSE;
			}
		}

		Sleep(100);
	}
}

// Procedimento para imprimir as informacoes das hotkeys usadas no procedimento HotkeyLoop
void TOOL::printHotkeyInfo (void) {
	system("CLS");
	printf("\n|----------------------------------------------------------------------------|");
	printf("\n|  List Threads (CRTL+1) | Pause Threads (CRTL+2) | Resume Threads (CTRL+3)  |");
	printf("\n|----------------------------------------------------------------------------|");
	printf("\n|  Read Memory (CTRL+4)  | Write Memory (CTRL+5)  | InjectDll (CTRL+6)       |");
	printf("\n|----------------------------------------------------------------------------|");
	printf("\n|  List Modules (CTRL+7) | ListThrdsStartAddrOff (CTRL+8)                    |");
	printf("\n|----------------------------------------------------------------------------|");
	printf("\n|  Quit (CTRL+0)                                                             |");
	printf("\n|----------------------------------------------------------------------------|\n");
}

// Procedimento que implementa um loop que monitora teclas do teclado possibilitando o uso de Hotkeys
void TOOL::HotkeyLoop(DWORD procPID) {
	BOOL check = TRUE;

	uintptr_t procMemAddr = 0x0;
	int procMemData = 0;

	char pathdll [MAX_PATH];
	//char pathtest [] = "C:\dll.dll";

	printHotkeyInfo();

	while (check) {

		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x30) < 0)) {
			printf("\n\nKey pressed: Quit (CTRL+0)\n\n");
			check = FALSE;
			//break;
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x31) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+1");
			ProcUtils::Print.ThreadsList(procPID);
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x32) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+2");
			if (PauseThreads(procPID) == 0) printf("\n\n  Sucess => All threads paused of PID: %lu", procPID);
			else printf("\n\nFail => Pausing any thread of PID: %lu", procPID);
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x33) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+3");
			if (ResumeThreads(procPID) == 0) printf("\n\n  Sucess => All threads resumed of PID: %lu", procPID);
			else printf("\n\n  Fail => Resuming any thread of PID: %lu", procPID);
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x34) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+4\n");
			printf ("\n\nInform the Address to Read an integer: ");
			fflush(stdin);
			std::cin >> std::hex >> procMemAddr; //scanf("%x", &procMemAddr);
			//printf ("\n%X", procMemAddr);
			if (ProcUtils::Memory.ReadProcMem_INT (procPID, procMemAddr, procMemData) == EXIT_SUCCESS) {
				printf("  Sucess => Reading process memory of PID: %lu", procPID);
				//printf("\nThe value on Address (%p) = %d", procMemAddr, procMemData);
				std::cout << "\n  The value on Address (0x" << std::hex << std::uppercase << (uintptr_t) procMemAddr << ") = " << std::dec << procMemData;
			} else {
				printf("\n\n  Fail => Reading process memory of PID: %lu", procPID);
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x35) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+5\n");
			printf ("\n\nInform the Address to Write an integer: ");
			fflush(stdin);
			std::cin >> std::hex >> procMemAddr;
			printf ("\n\nInform the value to Write: ");
			fflush(stdin);
			std::cin >> std::dec >> procMemData;
			if (ProcUtils::Memory.WriteProcMem_INT (procPID, procMemAddr, procMemData) == EXIT_SUCCESS) {
				printf("  Sucess => Writing process memory of PID: %lu", procPID);
			} else {
				printf("\n\n  Fail => Writing process memory of PID: %lu", procPID);
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x36) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+6\n");
			printf ("\n\nInform the path of dll: ");
			fflush(stdin);
			std::cin >> pathdll;
			correctPath(pathdll);
			if (InjectDll (procPID, pathdll) != EXIT_FAILURE) {
				printf("  Sucess => Injecting Dll on Process ID: %lu", procPID);
			} else {
				printf("\n\n  Fail => Injecting Dll on Process ID: %lu", procPID);
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x37) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+7");
			ProcUtils::Print.ModList(procPID);
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x38) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+8");
			ProcUtils::Print.ThrdsStartAddrOff(procPID);
		}
		if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(0x39) < 0)) {
			printHotkeyInfo();
			//printf("\nKeys pressed: CTRL+9");
			//ProcUtils::Get.ProcName(PID1, procTarget1);
			//ProcUtils::Tool.ProcMonitor(procTarget1);
		}

		Sleep(100);
	}

}

// Funcao que recebe um PID e um endereco de memoria desse processo e tenta ler um conteudo de tipo int;
// Retorna uma confirmacao de sucesso/falha e um paramentro de retorno contendo o dado lido;
BOOL MEMORY::ReadProcMem_INT (DWORD procPID, uintptr_t procAddr, int &rData) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procPID);
    if (hProcess == NULL) {
        std::cout << "OpenProcess Failed. GetLastError: " << std::dec << GetLastError() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
	BOOL retRPM = ReadProcessMemory(hProcess, (LPCVOID)procAddr, &rData, sizeof(int), NULL);
    if (retRPM == FALSE) {
        std::cout << "ReadProcessMemory failed. GetLastError = " << std::dec << GetLastError() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
	//cout << "The value(integer) at address is: " << dec << rData << endl;
    CloseHandle(hProcess);
    return EXIT_SUCCESS;
}

// Funcao que recebe um PID, um endereco de memoria desse processo e um numero inteiro para ser escrito
// na memoria deste processo; Retorna uma confirmacao de sucesso/falha;
BOOL MEMORY::WriteProcMem_INT (DWORD procPID, uintptr_t procAddr, int &wData) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procPID);
    if (hProcess == NULL) {
        std::cout << "OpenProcess Failed. GetLastError: " << std::dec << GetLastError() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
	BOOL retRPM = WriteProcessMemory(hProcess, (LPVOID)procAddr, &wData, sizeof(int), NULL);
    if (retRPM == FALSE) {
        std::cout << "WriteProcessMemory failed. GetLastError = " << std::dec << GetLastError() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
    CloseHandle(hProcess);
    return EXIT_SUCCESS;
}

// Funcao que recebe um Process ID e um caminho que contem uma DLL, e serve para injetar essa dll no processo
BOOL TOOL::InjectDll (DWORD procPID, char* dllpath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procPID);
	if (hProcess == NULL) {
        std::cout << "OpenProcess Failed. GetLastError: " << std::dec << GetLastError() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
    LPVOID Ploadlibrary = (LPVOID) GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");
    LPVOID Pcaminho = VirtualAllocEx(hProcess, NULL, strlen(dllpath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, Pcaminho, dllpath, strlen(dllpath), NULL);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)Ploadlibrary, Pcaminho, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, Pcaminho, strlen(dllpath), MEM_RELEASE);
	if (hThread != INVALID_HANDLE_VALUE) {
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return EXIT_SUCCESS;
	} else {
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
}

// Funcao para correcao de entradas de filepath de usuario.
// (ex: C:\usuario1\diretorio1\aquivo1.txt) => (ex: C:\\usuario1\\diretorio1\\aquivo1.txt)
void TOOL::correctPath (char* pathIn) {
	int i=0,c=0;
	char temp [MAX_PATH];

	for (i=0;pathIn[i] != '\0';i++) {
		temp[i+c] = pathIn[i];
		if (pathIn[i] == '\\') {		// se o caracter for igual a barra
			c++;						// soma o contador
			temp[i+c] = '\\';			// atribui mais um caracter barra para a string resultante
		}
	}
	temp[i+c] = '\0';

	for (i=0;temp[i] != '\0';i++) {		// atrubui a string original a string resultante
		pathIn[i] = temp[i];
	}
	pathIn[i] = '\0';
}
