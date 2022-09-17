#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <vector>
#include "File.h"

#define NORMAL 0
#define COMMAND 1
#define INSERT 2
#define SEARCH 3
#define HELP 4

#define ALL_ROWS 40
#define ALL_COLUMNS 155

#define MODE_L 20
#define NAME_L 115
#define NUMBER_L 20
#define CONS_H 2


//Subcriber
class Observer 
{
public:
	virtual void update_com(MyString& content, const int symb, const int com_index, const unsigned char win) = 0;
	virtual void update_text(File& content, const int symb, int& x, int& y, const size_t& string_num) = 0;
	virtual void update_stat(MyString filename, const size_t cur_str, const size_t str_am) = 0;
	virtual void update_mode(int& mode) = 0;
	virtual void update_cursor(int x, int y) = 0;
	virtual void new_file() = 0;
	virtual void back_to_text(int& x, int& y) = 0;
	virtual void set_new_start(const size_t index, int& x, int& y) = 0;
};

//Publisher
class Observable
{
public:
	virtual void notify_update_com(MyString& content, const int symb, const int str_index, const unsigned char win)
	{
		for (auto& observer : observers)
			observer->update_com(content, symb, str_index, win);
	}
	virtual void notify_update_text(File& content, const int symb, int& x, int& y, const size_t& string_num)
	{
		for (auto& observer : observers)
			observer->update_text(content, symb, x, y, string_num);
	}
	virtual void notify_update_stat(MyString filename, const size_t cur_str, const size_t str_am)
	{
		for (auto& observer : observers)
			observer->update_stat(filename, cur_str, str_am);
	}
	virtual void notify_update_mode(int& mode)
	{
		for (auto& observer : observers)
			observer->update_mode(mode);
	}
	virtual void notify_update_cursor(int x, int y)
	{
		for (auto& observer : observers)
			observer->update_cursor(x, y);
	}
	virtual void notify_new_file()
	{
		for (auto& observer : observers)
			observer->new_file();
	}
	virtual void notify_back_to_text(int& x, int& y)
	{
		for (auto& observer : observers)
			observer->back_to_text(x, y);
	}
	virtual void notify_set_new_start(const size_t index, int& x, int& y)
	{
		for (auto& observer : observers)
			observer->set_new_start(index, x, y);
	}

	virtual void add_observer(Observer* observer)
	{
		observers.push_back(observer);
	}

protected:
	vector<Observer*> observers;
};

#endif //__OBSERVER_H__