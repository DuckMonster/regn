#include "select_process.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <wchar.h>
#include "console.h"

#define MAX_PROCESSES 1024
Process** process_list;
int process_num;
int* index_list;
int index_num;

int selected_index = 0;
int enum_start_idx = 0;

void fill_process_list()
{
	process_list = new Process*[MAX_PROCESSES];
	index_list = new int[MAX_PROCESSES];

	DWORD process_ids[MAX_PROCESSES];
	DWORD out_size;
	EnumProcesses(process_ids, sizeof(process_ids), &out_size);

	int idx = 0;
	process_num = out_size / sizeof(DWORD);

	for(int i=0; i<process_num; ++i)
	{
		Process* proc = proc_open(process_ids[i], PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
		if (!proc)
			continue;

		process_list[idx] = proc;
		index_list[idx] = idx;
		
		idx++;
	}

	// See how many processes we _actually_ saved (ignoring ones we couldnt open)
	process_num = index_num = idx;
}

void print_header()
{
	console_clear_line();
	console_set_bg_clr(CLR_Yellow, false);
	console_set_fg_clr(CLR_Black, false);
	printf(" SELECT PROCESS TO ATTACH \n");

	console_reset_clr();
}

void print_process_list()
{
	int start = enum_start_idx;
	int end = min(enum_start_idx + 20, index_num);

	for(int i=start; i<end; ++i)
	{
		Process* proc = process_list[index_list[i]];
		if (i == selected_index)
		{
			console_set_bg_clr(CLR_White, true);
			console_set_fg_clr(CLR_Black, false);
		}
		else
		{
			console_set_fg_clr(CLR_White, false);
			console_set_bg_clr(CLR_Black, false);
		}

		console_clear_line();
		printf("[%5d]\t%ls\n", proc->id, proc_name(proc));
	}

	console_reset_clr();
}

void find()
{
	/*
	printf("Search str: ");
	const TCHAR* filter = input_get_string();
	enum_processes([=](Process* proc)
	{
		if (wcsstr(proc_name(proc), filter) != nullptr)
			print_process(proc);
	});
	*/
}

Process* attach()
{
	/*
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
	*/
	return nullptr;
}

void select_index(int new_index)
{
	selected_index = new_index;
	selected_index = min(selected_index, index_num);
	selected_index = max(selected_index, 0);

	if (selected_index - 2 < enum_start_idx)
		enum_start_idx = max(selected_index - 2, 0);

	else if (selected_index + 2 > enum_start_idx + 20)
		enum_start_idx = min(selected_index + 2 - 20, index_num - 20);
}

Process* select_process()
{
	console_clear();

	fill_process_list();

	while(true)
	{
		console_set_cursor_pos(0, 0);
		print_header();
		print_process_list();

		Key key = console_get_key();
		switch(key.key_code)
		{
			case 0x4A:
				select_index(selected_index + 1);
				break;

			case 0x4B:
				select_index(selected_index - 1);
				break;
		}
	}

	return nullptr;
}