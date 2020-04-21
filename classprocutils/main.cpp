
#include "pch.h"


//__declspec(dllexport)

int main () {

	char procTarget[MAX_PATH] = { 0 };
	
	//wchar_t procTargetW[MAX_PATH]={0};
    //wchar_t procTarget[50] = L""; // wchar_t procTarget[] = L"Target.exe";
	//wchar_t moduleTarget[] = L"kernel32.dll";
	DWORD PID;
	//DWORD *PIDs;
	//BYTE* moduleBaseAddr;
	//DWORD moduleBaseSize;

	system("Title Usando lib TlHelp32");

	printf("\n------  Lista de Processos  ------\n\n");
	ProcUtils::Print.ProcList ();
	
	//printf("\n\nDigite o nome do processo: ");
	//std::cin >> procTarget;
	//std::wcin >> procTargetW; //wscanf_s(L"%ls", procTarget);

	printf("\n\nDigite o PID do processo: ");
	std::cin >> PID; //scanf("%d", &PID);	
	//printf("%d\n", PID);
    //ProcUtils::Get.ProcName(PID, procTarget);
    //printf(" -> %s\n", procTarget);


	if ((ProcUtils::Get.ProcName (PID, procTarget)) == EXIT_SUCCESS) {	//if ((PID = GetProcPID(procTarget)) != 0) {		

		//printf("\n  -  Base Address do processo  -\n");
		//printf("\n  BaseAddr: 0x%X", ProcUtils::Get.ModBaseAddr(PID, procTarget));
		
		//ProcUtils::Print.ThreadsList (PID);

        //ProcUtils::Tool.PauseAndResumeThreads(PID);

		//ProcUtils::Tool.HotkeyLoop (PID);
		
		ProcUtils::Tool.ProcMonitor(procTarget);


	} else
		printf("Processo nao encontrado!\n");


	system("pause");
	return 0;
}
