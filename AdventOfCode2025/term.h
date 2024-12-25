// term.h: Console (terminal) utility functions

#include <wincon.h>
#include <conio.h>

#ifndef _TERM_H
#define _TERM_H

inline void gotoxy(short x, short y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

inline void getxy(short& x, short& y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(h, &info);
	x = info.dwCursorPosition.X;
	y = info.dwCursorPosition.Y;
}

// Colour codes from https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
enum class TermColour
{
	Default = 0,
	Bold = 1,
	Underline = 4,
	Negative = 7,

	NoUnderline = 24,
	Positive = 27,

	Black = 30,
	Red = 31,
	Green = 32,
	Yellow = 33,
	Blue = 34,
	Magenta = 35,
	Cyan = 36,
	LightGrey = 37,

	BlackBG = 40,
	RedBG = 41,
	GreenBG = 42,
	YellowBG = 43,
	BlueBG = 44,
	MagentaBG = 45,
	CyanBG = 46,
	LightGreyBG = 47,

	DarkGrey = 90,
	LightRed = 91,
	LightGreen = 92,
	LightYellow = 93,
	LightBlue = 94,
	LightMagenta = 95,
	LightCyan = 96,
	White = 97,

	DarkGreyBG = 100,
	LightRedBG = 101,
	LightGreenBG = 102,
	LightYellowBG = 103,
	LightBlueBG = 104,
	LightMagentaBG = 105,
	LightCyanBG = 106,
	WhiteBG = 107,
};

inline void setcolour(TermColour termColour)
{
	printf("\033[%dm", termColour);
}

#endif // _TERM_H