#ifndef __PDCURSES_H__
#define __PDCURSES_H__

#include <iostream>
#include <vector>
#include <curses.h>

using namespace std;

class TUIInterface
{
public:
    virtual void move_cursor(int x, int y) = 0;

    virtual void print(WINDOW* win, int y, int x, const char* str) = 0;
    virtual void print(WINDOW* win, int x, int y, char c) = 0;
    virtual void print(WINDOW* win, int y, int x, const size_t cur_str, const size_t str_am) = 0;
   
    virtual void create_color_id(short, short, short) = 0;
    virtual int resize(int height, int windth) = 0;

    virtual int _wclear(WINDOW* window) = 0;
    virtual WINDOW* _newwin(int nrows, int ncol, int y, int x) = 0;
    virtual int _wbkgd(WINDOW* window, chtype color) = 0;
    virtual int _wrefresh(WINDOW* window) = 0;
    virtual void delwin(WINDOW* windows) = 0;
};

class PDCursesAdapter : public TUIInterface
{
public:
    PDCursesAdapter();

    void move_cursor(int x, int y) override;

    void print(WINDOW* win, int y, int x, const char* str);
    void print(WINDOW* win, int x, int y, char c);
    void print(WINDOW* win, int y, int x, const size_t cur_str, const size_t str_am);

    void create_color_id(short id, short text_color, short background_color);
    int resize(int height, int windth);

    int _wclear(WINDOW* window);
    WINDOW* _newwin(int nrows, int ncol, int y, int x);
    int _wbkgd(WINDOW* window, chtype color);
    int _wrefresh(WINDOW* window);
    void delwin(WINDOW* windows);
};

#endif //__PDCURSESADAPTER_H__