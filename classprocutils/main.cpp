
#include "pch.h"


__declspec(dllexport)

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
    //GetProcName(PID, procTarget);
    //printf(" -> %s\n", procTarget);
	//GetProcNameW(PID, procTargetW);
   // printf(" -> %S\n", procTargetW);


	if ((ProcUtils::Get.ProcName (PID, procTarget)) == EXIT_SUCCESS) {	//if ((PID = GetProcPID(procTarget)) != 0) {
	//if ((PID = GetProcPIDW(procTargetW)) != 0) {	//if (PID == GetProcPIDW (procTarget)) {
		//printf("Processo encontrado!\n");
		//printf("ID: %lu \n", PID);

/*
		system("CLS");
		printf("\n--------PIDs encontrados--------\n\n");
		PrintPIDsByNameW(procTarget);
		printf("\n--------------------------------\n");
*/
		
		//printf("\n  -  Lista de modulos do processo (%s) PID (%lu):  -\n", procTarget, PID);
		////printf("\n  -  Lista de modulos do processo (%S) PID (%d):  -\n", procTargetW, PID);
		//PrintModList(PID);

		//printf("\n  -  Base Address do processo  -\n");
		//printf("\n  BaseAddr: 0x%X", GetModBaseAddr(PID, procTarget));
		
		//PrintThreadsList (PID);

        //PauseAndResumeThreads(PID);

		//ProcUtils::Tool.HotkeyLoop (PID);
		
		ProcUtils::Tool.ProcMonitor(procTarget);


	} else
		printf("Processo nao encontrado!\n");


	system("pause");
	return 0;
}
