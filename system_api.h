#ifndef SYSTEM_API_H
#define SYSTEM_API_H

#include <stdio.h>

#define C_COLOR_BLACK "\033[22;30m"
#define C_COLOR_RED "\033[22;31m"
#define C_COLOR_GREEN "\033[22;32m"
#define C_COLOR_BROWN "\033[22;33m"
#define C_COLOR_BLUE "\033[22;34m"
#define C_COLOR_MAGENTA "\033[22;35m"
#define C_COLOR_CYAN "\033[22;36m"
#define C_COLOR_GRAY "\033[22;37m"
#define C_COLOR_DARK_GRAY "\033[01;30m"
#define C_COLOR_LIGHT_RED "\033[01;31m"
#define C_COLOR_LIGHT_GREEN "\033[01;32m"
#define C_COLOR_YELLOW "\033[01;33m"
#define C_COLOR_LIGHT_BLUE "\033[01;34m"
#define C_COLOR_LIGHT_MAGENTA "\033[01;35m"
#define C_COLOR_WHITE "\033[01;37m"
#define C_COLOR_RESET "\033[0m"

#ifdef _WIN32
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

HANDLE stdoutHandle;
DWORD outModeInit;

void setup_console_colors();

void restore_console_colors();

void set_console_color(char* color);

void clear_screen(int lines);

void clear_rest_screen(int lines);

void system_sleep(int time_in_seconds);

int get_terminal_height();

int get_terminal_width();


#else

#include <unistd.h>
#include <sys/ioctl.h>

void setup_console_colors();

void restore_console_colors();

void set_console_color(char* color);

void clear_screen(int lines);

void clear_rest_screen(int lines);

void system_sleep(int time_in_seconds);

int get_terminal_height();

int get_terminal_width();

#endif

#endif
