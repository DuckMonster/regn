#include "process_debug.h"
#include <windows.h>
#include <stdio.h>
#include "console.h"

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
	// Start debugging
	DebugActiveProcess(proc->id);
	while(true)
		console_get_key();
}
