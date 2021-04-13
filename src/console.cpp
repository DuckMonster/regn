#include "console.h"
#include <stdio.h>
#include <windows.h>

#define ESC "\x1b["
HANDLE output_handle;
HANDLE input_handle;

void console_init()
{
	output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	input_handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD console_mode;
	GetConsoleMode(output_handle, &console_mode);

	console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(output_handle, console_mode);

	// Always disable blinking
	printf(ESC "?12l");
	console_hide_cursor();
}

void console_set_cursor_pos(int x, int y)
{
	printf(ESC "%d;%dH", y + 1, x + 1);
}

void console_set_cursor_col(int x)
{
	printf(ESC "%dG", x + 1);
}

void console_clear()
{
	printf(ESC "2J");
}

void console_clear_line()
{
	printf(ESC "2K");
}

void console_show_cursor()
{
	printf(ESC "?25h");
}

void console_hide_cursor()
{
	printf(ESC "?25l");
}

void console_reset_clr()
{
	printf(ESC "0m");
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
		if (!ReadConsoleInput(input_handle, &input, 1, &num_inputs))
		{
			printf("Failed to read console input...\n");
			continue;
		}

		// Only read key events
		if (input.EventType != KEY_EVENT)
			continue;

		KEY_EVENT_RECORD* key_event = &input.Event.KeyEvent;

		// Only read pressed keys
		if (!key_event->bKeyDown)
			continue;

		result.key_code = key_event->wVirtualKeyCode;
		result.scan_code = key_event->wVirtualScanCode;
		result.character = key_event->uChar.AsciiChar;
		result.wcharacter = key_event->uChar.UnicodeChar;

		return result;
	}
}

Key console_get_key_noblock()
{
	return Key();
}