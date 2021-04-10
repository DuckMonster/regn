#pragma once
#include <windows.h>
#define MAX_MODULES 128

struct Module
{
	HMODULE handle;
	TCHAR* name = nullptr;

	void* base_ptr;
	int size;
	void* entry_point;
};

struct Process
{
	int id;
	HANDLE handle;
	Module modules[MAX_MODULES];
	int num_modules;
};

Process* proc_open(int id, int flags);
void proc_close(Process* proc);

const TCHAR* proc_name(Process* proc);