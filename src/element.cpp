#include<windows.h>

#include "element.h"

RECT set_screen_bounds() {
	RECT screen = {};
	SystemParametersInfoW(SPI_GETWORKAREA, 0, &screen, 0);
	
	// Check Taskbar Behaviour
	APPBARDATA abd = { sizeof(APPBARDATA) };
	UINT state = (UINT)SHAppBarMessage(ABM_GETSTATE, &abd);
	if ((state & ABS_AUTOHIDE) != 0) {
		screen.bottom -= 5;
	}

	return screen;
}

RECT set_terminal_bounds(RECT screen) {
	RECT terminal = {};
	const int margin = 20;

	terminal.left = screen.left + margin;
	terminal.top = screen.bottom - 400;
	terminal.right = screen.right - margin;
	terminal.bottom = screen.bottom - margin;

	return terminal;
}