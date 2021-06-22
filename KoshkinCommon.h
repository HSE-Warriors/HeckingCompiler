#pragma once

#define COLOR_RED     "\x1b[31m" //color 1
#define COLOR_YELLOW  "\x1b[33m" //color 2
#define COLOR_GREEN   "\x1b[32;1m" //color 3
#define COLOR_CYAN    "\x1b[36m" //color 4
#define COLOR_MAGENTA   "\x1b[35;1m" //color 5
#define COLOR_RESET   "\x1b[0m"  //color 0

void setLocale();
void enableScrolling();
void setColor(int color);
