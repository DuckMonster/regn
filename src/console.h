#pragma once
#include <tchar.h>

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

void console_show_cursor();
void console_hide_cursor();

void console_reset_clr();
void console_set_fg_clr(int clr, bool bright=false);
void console_set_bg_clr(int clr, bool bright=false);

enum Virtual_Key_Codes
{
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z,
};

struct Key
{
	int key_code;
	int scan_code;
	char character;
	TCHAR wcharacter;
};

Key console_get_key();
Key console_get_key_noblock();