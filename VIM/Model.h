#ifndef __MODEL_H__
#define __MODEL_H__

#include "Observer.h"
#include "PDCurses.h"

#define BACKSPACE  8
#define DELETE 330
#define ENTER 10
#define ESC 27

#define CONS 1
#define TEXT 2
#define FILENAME 3
#define TEXT_CONS 4
#define SEAR 5

using namespace std;

class Model :public Observable 
{
public:
	Model();

	bool is_command(MyString& command);
	void move_str_start();
	void move_str_end();
	void move_to_word(const MyString&);
	void move_str_by_num(MyString& buttons);
	void page_down();
	void page_up();
	void help();

	bool move_left();
	bool move_right();
	bool move_up();
	bool move_down();

	bool move_left_cons(size_t& index);
	bool move_right_cons(size_t& index, MyString& data);
	bool move_up_cons(size_t& index, MyString& data);
	bool move_down_cons(size_t& index, MyString& data);

	void recognize_symb(const int symb);
	void recog_command(const int symb);
	void recog_normal(const int symb);
	void recog_search(const int symb);
	void recog_help(const int symb);
	void recog_insert(const int symb);

private:
	int mode; // normal, insert, search, commands, help
	bool scroll_flag;

	//some variables for commands
	MyString cons_data;
	size_t cons_index;
	
	//some variables for searching
	size_t search_index;
	int x_search, y_search;
	MyString search_data;
	MyString prev_search_data;
	bool prev_search;

	MyString buttons; //pressed buttons for double commands
	File buffer_help;

	MyString string_copy; //for yy and yw
	MyString empty; //empty string
	
	File file; // loaded file
	int x_absolute, y_absolute; // in memory
	int x_real, y_real; // in screen
	MyString line_under; // iterator to line which under cursor
	int line_under_num; //number of current string
	size_t strings_amount; //count of all string
};

#endif //__MODEL_H__