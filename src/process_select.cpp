#include "process_select.h"
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
	console_set_cursor_pos(0, 0);
	console_clear_line();
	console_set_bg_clr(CLR_Yellow, false);
	console_set_fg_clr(CLR_Black, false);
	printf(" SELECT PROCESS TO ATTACH \n\n");

	console_reset_clr();
}

void print_process_list()
{
	console_set_cursor_pos(0, 2);
	int start = enum_start_idx;
	int end = enum_start_idx + 20;

	for(int i=start; i<end; ++i)
	{
		console_reset_clr();
		console_clear_line();

		if (i >= index_num)
		{
			printf("\n");
			continue;
		}

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

		printf("[%5d]\t%ls\n", proc->id, proc_name(proc));
	}

	console_reset_clr();
}

void select_index(int new_index)
{
	selected_index = new_index;
	selected_index = min(selected_index, index_num - 1);
	selected_index = max(selected_index, 0);

	if (selected_index - 2 < enum_start_idx)
		enum_start_idx = max(selected_index - 2, 0);

	else if (selected_index + 2 > enum_start_idx + 20)
		enum_start_idx = min(selected_index + 2 - 20, index_num - 20);
}

void apply_filter(const TCHAR* filter)
{
	index_num = 0;
	for(int i=0; i<process_num; ++i)
	{
		// Check filter
		if (wcsstr(proc_name(process_list[i]), filter) == nullptr)
			continue;

		index_list[index_num++] = i;
	}

	if (selected_index >= index_num)
	{
		selected_index = max(index_num - 1, 0);
		if (enum_start_idx > selected_index)
			enum_start_idx = selected_index;
	}
}

void reset_filter()
{
	index_num = process_num;
	for(int i=0; i<index_num; ++i)
		index_list[i] = i;
}

void set_filter_string()
{
	TCHAR filter_str[80];
	memset(filter_str, 0, 80);

	int str_len = 0;

	reset_filter();

	while(true)
	{
		console_set_cursor_pos(0, 1);
		console_clear_line();

		console_set_fg_clr(CLR_Black);
		console_set_bg_clr(CLR_Magenta, true);
		printf(" Filter: %ls", filter_str);

		console_reset_clr();

		console_show_cursor();
		Key key = console_get_key();
		console_hide_cursor();

		// Writing character
		if (key.character >= ' ' && key.character <= '~')
			filter_str[str_len++] = key.wcharacter;

		// Backspace
		if (key.character == '\b' && str_len > 0)
			filter_str[--str_len] = 0;

		// Apply
		if (key.key_code == VK_RETURN)
		{
			return;
		}

		// Go back
		if (key.key_code == VK_ESCAPE)
		{
			reset_filter();
			return;
		}

		apply_filter(filter_str);
		print_process_list();
	}
}

Process* select_process()
{
	console_clear();

	fill_process_list();

	while(true)
	{
		print_header();
		print_process_list();

		Key key = console_get_key();
		switch(key.key_code)
		{
			case VK_F:
				set_filter_string();
				break;

			case VK_J:
				select_index(selected_index + 1);
				break;

			case VK_K:
				select_index(selected_index - 1);
				break;

			case VK_RETURN:
				return process_list[index_list[selected_index]];
		}
	}

	return nullptr;
}