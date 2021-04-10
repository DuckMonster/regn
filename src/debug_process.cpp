#include "debug_process.h"
#include <windows.h>
#include <stdio.h>
#include "input.h"

void flush_events()
{
	DEBUG_EVENT event;
	while(WaitForDebugEvent(&event, 50))
	{
		ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
		printf("Flushing event %d...\n", event.dwDebugEventCode);
	}
}

void break_process(Process* proc)
{
	if (!DebugBreakProcess(proc->handle))
	{
		printf("Failed to break: %d\n", GetLastError());
	}
}

void continue_process(Process* proc)
{

}

void debug_process(Process* proc)
{
	do
	{
		flush_events();

		printf("(B)reak, (C)ontinue: ");
		char option = input_get_char();
		printf("\n");

		switch(option)
		{
			case 'b':
			case 'B':
				break_process(proc);
				break;

			case 'c':
			case 'C':
				continue_process(proc);
				break;

			default:
				printf("Unknown command '%c'\n", option);
				break;
		}
	} while(true);
}
