#include "system_api.h"

#ifdef _WIN32

void setup_console_colors(){
    DWORD outMode = 0;
 	stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
 	if(stdoutHandle == INVALID_HANDLE_VALUE) {
 		exit(GetLastError());
 	}
 	if(!GetConsoleMode(stdoutHandle, &outMode)) {
 		exit(GetLastError());
 	}
 	outModeInit = outMode;
 	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
 	if(!SetConsoleMode(stdoutHandle, outMode)) {
 		exit(GetLastError());
 	}
}

void restore_console_colors() {
    set_console_color(C_COLOR_RESET);
 	if(!SetConsoleMode(stdoutHandle, outModeInit)) {
 		exit(GetLastError());
 	}
}

void set_console_color(char* color){
    printf("%s", color);
}

void clear_screen(int lines){
    int i;
    for(i = 0; i < lines; i++)
        printf("\n");
}

void clear_rest_screen(int lines){
    clear_screen(get_terminal_height() - lines);
}

void system_sleep(int time_in_miliseconds){
    Sleep(time_in_miliseconds*1000);
}

int get_terminal_height(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

int get_terminal_width(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return (csbi.srWindow.Right - csbi.srWindow.Left + 1);
}

#else

void setup_console_colors(){ }

void restore_console_colors(){
    set_console_color(C_COLOR_RESET);
}

void set_console_color(char* color){
    printf("%s", color);
}

void clear_screen(int lines){
    int i;
    for(i = 0; i < lines; i++)
        printf("\n");
}

void clear_rest_screen(int lines){
    clear_screen(get_terminal_height() - lines);
}

void system_sleep(int time_in_seconds){
    sleep(time_in_seconds);
}

int get_terminal_height(){
    struct winsize window_size;
    ioctl(0, TIOCGWINSZ , &window_size);
    return window_size.ws_row;
}
int get_terminal_width(){
    struct winsize window_size;
    ioctl(0, TIOCGWINSZ , &window_size);
    return window_size.ws_col;
}

#endif
