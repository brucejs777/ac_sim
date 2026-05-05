/* app_ansi.h
ansi control codes for formatted output
note: in web client terminal, use xterm.js to allow these
*/

#ifndef APP_ANSI_H
#define APP_ANSI_H

// Cursor control
#define ANSI_HOME       "\033[H"
#define ANSI_CLEAR      "\033[2J"
#define ANSI_CLEAR_LINE "\033[2K"
#define ANSI_SAVE_CUR   "\033[s"
#define ANSI_REST_CUR   "\033[u"

// Cursor movement
#define ANSI_UP(n)      "\033[" #n "A"
#define ANSI_DOWN(n)    "\033[" #n "B"
#define ANSI_RIGHT(n)   "\033[" #n "C"
#define ANSI_LEFT(n)    "\033[" #n "D"

// Colors - Foreground
#define ANSI_FG_RED     "\033[31m"
#define ANSI_FG_GREEN   "\033[32m"
#define ANSI_FG_YELLOW  "\033[33m"
#define ANSI_FG_BLUE    "\033[34m"
#define ANSI_FG_WHITE   "\033[37m"
#define ANSI_FG_DEFAULT "\033[39m"

// Colors - Background
#define ANSI_BG_RED     "\033[41m"
#define ANSI_BG_GREEN   "\033[42m"
#define ANSI_BG_YELLOW  "\033[43m"
#define ANSI_BG_DEFAULT "\033[49m"

// Text style
#define ANSI_BOLD       "\033[1m"
#define ANSI_DIM        "\033[2m"
#define ANSI_UNDERLINE  "\033[4m"
#define ANSI_RESET      "\033[0m"

#endif // APP_ANSI_H
