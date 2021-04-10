#include "process.h"
#include <psapi.h>

Process* proc_open(int id, int flags)
{
	// Open actual process
	HANDLE handle = OpenProcess(flags, false, id);
	if (!handle)
		return nullptr;

	Process* proc = new Process();
	proc->id = id;
	proc->handle = handle;

	// Read modules
	HMODULE module_handles[MAX_MODULES];
	DWORD module_size_read;
	if (EnumProcessModules(handle, module_handles, sizeof(module_handles), &module_size_read))
	{
		proc->num_modules = module_size_read / sizeof(HMODULE);
		if (proc->num_modules > MAX_MODULES)
			proc->num_modules = MAX_MODULES;

		for(int i=0; i<proc->num_modules; i++)
		{
			Module& module = proc->modules[i];
			module.handle = module_handles[i];

			// Fetch name
			module.name = (TCHAR*)malloc(1024);
			GetModuleBaseName(proc->handle, module.handle, module.name, 1024);

			// Fetch memory stuff
			MODULEINFO info;
			if (GetModuleInformation(proc->handle, module.handle, &info, sizeof(info)))
			{
				module.base_ptr = info.lpBaseOfDll;
				module.size = info.SizeOfImage;
				module.entry_point = info.EntryPoint;
			}
		}
	}

	return proc;
}

void proc_close(Process* proc)
{
	CloseHandle(proc->handle);
	delete proc;
}

const TCHAR* proc_name(Process* proc)
{
	return proc->modules[0].name;
}