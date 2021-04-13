#pragma once

enum Console_Color
{
	CLR_Black,
	CLR_Red,
	CLR_Green,
	CLR_Yellow,
	CLR_Blue,
	CLR_Magenta,
	CLR_Cyan,
	CLR_White,
	CLR_Default = 9,
};

void console_init();
void console_set_cursor_pos(int x, int y);
void console_set_cursor_col(int x);

void console_clear();
void console_clear_line();

void console_reset_clr();
void console_set_fg_clr(int clr, bool bright=false);
void console_set_bg_clr(int clr, bool bright=false);

struct Key
{
	int key_code;
	int scan_code;
	char character;
};

Key console_get_key();
Key console_get_key_noblock();