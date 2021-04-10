#include "process.h"
#include "select_process.h"
#include "debug_process.h"
#include "console.h"

int main()
{
	console_init();
	Process* proc = select_process();
	debug_process(proc);
}