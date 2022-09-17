#include "PDCurses.h"

PDCursesAdapter::PDCursesAdapter()
{
    initscr();
    noecho();
    refresh();
    keypad(stdscr, TRUE);

    start_color();
}

void PDCursesAdapter::move_cursor(int y, int x)
{
    move(y, x);
}

void PDCursesAdapter::print(WINDOW* win, int y, int x, const char *str)
{
    mvwprintw(win, y, x, " %s ", str);
}

void PDCursesAdapter::print(WINDOW* win, int y, int x, char c)
{
    mvwprintw(win, y, x, "%c", c);
}

void PDCursesAdapter::print(WINDOW* win, int y, int x, const size_t cur_str, const size_t str_am)
{
    mvwprintw(win, y, x, " CUR:%d ALL:%d ", cur_str, str_am);
}

void PDCursesAdapter::create_color_id(short id, short text_color, short background_color)
{
    init_pair(id, text_color, background_color);
}

int PDCursesAdapter::resize (int height, int windth) 
{ 
    return resize_term(height, windth);
}

int PDCursesAdapter::_wclear(WINDOW* window) 
{ 
    return wclear(window); 
}

WINDOW* PDCursesAdapter::_newwin(int nrows, int ncol, int y, int x)
{
    return newwin(nrows, ncol, y, x);
}

int PDCursesAdapter::_wbkgd(WINDOW* window, chtype color)
{ 
    return wbkgd(window, color);
}

int  PDCursesAdapter::_wrefresh(WINDOW* window) 
{
    return wrefresh(window);
}

void PDCursesAdapter::delwin(WINDOW* windows)
{
    delwin(windows);
    endwin();
}