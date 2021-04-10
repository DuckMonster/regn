#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char input_get_char()
{
	while(true)
	{
		INPUT_RECORD input_buffer[1024];
		HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
		DWORD num_inputs;

		if (!ReadConsoleInput(stdin_handle, input_buffer, 1024, &num_inputs))
		{
			printf("Failed to read console input\n");
			return 0;
		}
		else
		{
			for(DWORD i=0; i<num_inputs; ++i)
			{
				INPUT_RECORD* input = &input_buffer[i];
				if (input->EventType != KEY_EVENT)
					continue;

				KEY_EVENT_RECORD* key_event = &input->Event.KeyEvent;
				if (!key_event->bKeyDown)
					continue;

				if (key_event->wRepeatCount != 1)
					continue;

				return key_event->uChar.AsciiChar;
			}
		}
	}
}

const TCHAR* input_get_string()
{
	static TCHAR BUFFER[1024];
	scanf_s("%ls", BUFFER, 1024);

	return BUFFER;
}

int input_get_int()
{
	int nmbr;
	scanf_s("%d", &nmbr);

	return nmbr;
}