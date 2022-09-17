#ifndef __VIEW_H__
#define __VIEW_H__

#include "PDCurses.h"
#include "Observer.h"
#include "Controller.h"


class View :public Observer
{
public:
	View(Model* model, Controller* controller, TUIInterface* adapter);

	void show(int window);
	int scroll_down(File& file, const size_t string_num);
	int scroll_up(File& file, const size_t string_num);

	//Observer
	void update_com(MyString& str, const int symb, const int cons_index, const unsigned char window);
	void update_text(File& file, const int symb, int& x, int& y, const size_t& string_num);
	void update_stat(MyString filename, const size_t cur_str, const size_t str_am);
	void update_mode(int& mode);
	void update_cursor(int x, int y);
	void new_file();
	void back_to_text(int& x, int& y);
	void set_new_start(const size_t index, int& x, int& y);

private:
	class Block 
	{
	public:
		Block() = default;
		virtual void show(TUIInterface* adapter) = 0;
		WINDOW* create_new_block(const int height, const int width, const int y, const int x, TUIInterface* adapter, int n, int color1, int color2);
	protected:
		WINDOW* window;
		WINDOW* filename; //file name
		WINDOW* count_string; // curent sting / count string
	};

	class TextBlock : public Block
	{
	public:
		TextBlock() = default;
		TextBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter);
		void show(TUIInterface* adapter);
		void put_data(File& file, TUIInterface* adapter);
		
		size_t get_start();
		void set_start(const size_t new_start);
	private:
		size_t text_out_str;
	};

	class ConsBlock : public Block
	{
	public:
		ConsBlock() = default;
		ConsBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter);
		void show(TUIInterface* adapter);
		void put_data(MyString& str, TUIInterface* adapter);
	};

	class StatusBlock : public Block
	{
	public:
		StatusBlock() = default;
		StatusBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter);
		void show(TUIInterface* adapter);
		void put_data(MyString& filename, const size_t cur_str, const size_t str_am, TUIInterface* adapter);
		void put_mode(int& mode, TUIInterface* adapter);
	};
	
	//командная строка
	int cons_y;
	int cons_x;

	int start;

	TextBlock text_block;
	ConsBlock cons_block;
	StatusBlock status_block;

	Model* model;
	Controller* controller;
	TUIInterface* adapter;
};

#endif //__VIEW_H__