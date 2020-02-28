#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
//#include <winternl.h>

#include "procutils.h"

//using namespace std;

#define MAX_PIDs  100
#define MAX_TIDs  200

#define MALLOC(ptr, size) { \
	ptr = (DWORD*) malloc(size); \
	if (ptr == NULL) { \
		fprintf(stderr,"memoria insuficiente\n"); \
		exit(1); \
	} \
}
