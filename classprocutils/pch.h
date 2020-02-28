#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
//#include <winternl.h>

#include "procutils.h"

//using namespace std;

#define MALLOC(ptr, size) { \
	ptr = (DWORD*) malloc(size); \
	if (ptr == NULL) { \
		fprintf(stderr,"memoria insuficiente\n"); \
		exit(1); \
	} \
}