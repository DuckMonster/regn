#include "select_process.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <wchar.h>
#include "input.h"

template<typename CallbackType>
void enum_processes(CallbackType callback)
{
	DWORD process_ids[1024];
	DWORD num_processes;

	EnumProcesses(process_ids, sizeof(process_ids), &num_processes);
	num_processes /= sizeof(DWORD);

	for(int i=0; i<num_processes; ++i)
	{
		Process* proc = proc_open(process_ids[i], PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
		if (!proc)
			continue;

		callback(proc);
		proc_close(proc);
	}
}

void print_process(Process* proc)
{
	printf("[%5d]\t%ls\n", proc->id, proc_name(proc));
}

void show_all()
{
	enum_processes([](Process* proc)
	{
		print_process(proc);
	});
}

void find()
{
	printf("Search str: ");
	const TCHAR* filter = input_get_string();
	enum_processes([=](Process* proc)
	{
		if (wcsstr(proc_name(proc), filter) != nullptr)
			print_process(proc);
	});
}

Process* attach()
{
	printf("Process id: ");
	int proc_id = input_get_int();

	Process* proc = proc_open(proc_id, PROCESS_ALL_ACCESS);
	if (!proc)
	{
		printf("Failed to open process with id %d\n", proc_id);
		return nullptr;
	}

	// Debug attach
	if (!DebugActiveProcess(proc->id))
	{
		printf("Failed to debug attach to process %d ('%ls')\n", proc_id, proc_name(proc));
		proc_close(proc);
		return nullptr;
	}

	DebugSetProcessKillOnExit(false);
	ContinueDebugEvent(proc->id, 0, 0);
	return proc;
}

Process* select_process()
{
	Process* result = nullptr;
	do
	{
		// Get option
		printf("(S)how all / (F)ind / (A)ttach: ");
		char option = input_get_char();
		printf("\n");

		switch(option)
		{
			case 'S':
			case 's':
				show_all();
				break;

			case 'F':
			case 'f':
				find();
				break;

			case 'A':
			case 'a':
				result = attach();
				break;

			default:
				printf("Unknown option '%c'\n", option);
				break;
		}

	} while(result == nullptr);

	return result;
}