#include "console.h"
#include <stdio.h>
#include <windows.h>

#define ESC "\x1b["
HANDLE console_handle;

void console_init()
{
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD console_mode;
	GetConsoleMode(console_handle, &console_mode);

	console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(console_handle, console_mode);
}

void console_set_cursor_pos(int x, int y)
{
	printf(ESC "%d;%dH", x, y);
}

void console_set_fg_clr(int clr, bool bright)
{
	printf(ESC "%dm", 30 + clr + (bright * 60));
}

void console_set_bg_clr(int clr, bool bright)
{
	printf(ESC "%dm", 40 + clr + (bright * 60));
}

Key console_get_key()
{
	Key result;
	while(true)
	{
		INPUT_RECORD input;
		DWORD num_inputs;
		ReadConsoleInput(console_handle, &input, 1, &num_inputs);

		// Only read key events
		if (input.EventType != KEY_EVENT)
			continue;

		KEY_EVENT_RECORD* key_event = &input.Event.KeyEvent;

		// Only read pressed keys
		if (!key_event->bKeyDown)
			continue;

		// And no repeats!!
		if (key_event->wRepeatCount != 1)
			continue;

		result.key_code = key_event->wVirtualKeyCode;
		result.scan_code = key_event->wVirtualScanCode;
		result.character = key_event->uChar.AsciiChar;

		return result;
	}
}

Key console_get_key_noblock()
{
	return Key();
}