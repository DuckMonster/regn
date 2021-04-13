#include "process.h"
#include "process_select.h"
#include "process_debug.h"
#include "console.h"

int main()
{
	console_init();
	Process* proc = select_process();
	debug_process(proc);
}