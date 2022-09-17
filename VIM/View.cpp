#include "View.h"

View::View(Model* _model, Controller* _controller, TUIInterface* _adapter)
{
	model = _model;
	controller = _controller;
	adapter = _adapter;

	start = 0;
	cons_y = 0;
	cons_x = 0;

	adapter->resize(ALL_ROWS, ALL_COLUMNS);
	text_block = TextBlock(ALL_ROWS - CONS_H, ALL_COLUMNS, 0, 0, adapter);
	cons_block = ConsBlock(CONS_H, ALL_COLUMNS, ALL_ROWS - CONS_H, 0, adapter);
	status_block = StatusBlock(CONS_H, ALL_COLUMNS, ALL_ROWS - CONS_H, 0, adapter);

	update_stat(MyString("__SAMPLE__.txt"), 1, 1);
	status_block.put_mode(start, adapter);
	status_block.show(adapter);
	model->add_observer(this);
}

int View::scroll_down(File& file, const size_t string_num)
{
	int num = text_block.get_start();
	int size, size_all = 0;
	int size_last = file.lines[string_num - 1].size() / ALL_COLUMNS + 1;

	while (size_last > 0)
	{
		size = file.lines[num].size();
		size = size / ALL_COLUMNS + 1;
		num++;
		size_last -= size;
		size_all += size;
	}

	text_block.set_start(num);
	return size_all;
}

int View::scroll_up(File& file, const size_t string_num)
{
	int num = text_block.get_start();
	int size_last = file.lines[string_num ].size() / ALL_COLUMNS + 1;

	text_block.set_start(num - 1);
	return size_last;
}

void View::show(int window)
{
	switch (window)
	{
	case TEXT:
		text_block.show(adapter);
		status_block.show(adapter);
		break;
	case TEXT_CONS:
		text_block.show(adapter);
		cons_block.show(adapter);
		break;
	case SEAR:
		status_block.show(adapter);
	case CONS:
		cons_block.show(adapter);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////

void View::update_mode(int& mode)
{
	status_block.put_mode(mode, adapter);
}

void View::update_com(MyString& str, const int symb, const int str_index, const unsigned char win)
{
	if (str.size() == 1 && (symb == ':' || symb == '?' || symb == '/')) 
	{
		cons_x = 1;
		cons_y = ALL_ROWS - CONS_H;
	}

	cons_block.put_data(str, adapter);
	show(win);

	switch (symb) 
	{
	case KEY_LEFT:
		if (cons_x > 0)
		{
			cons_x--;
		}
		else if (cons_y > ALL_ROWS - CONS_H)
		{
			cons_y--;
			cons_x = ALL_COLUMNS - 1;
		}
		break;
	case KEY_RIGHT:
		if (cons_x + 1 < ALL_COLUMNS)
		{
			cons_x++;
		}
		else if (cons_y < ALL_ROWS)
		{
			cons_y++;
			cons_x = 0;
		}
		break;
	case KEY_UP:
		if (cons_y > ALL_ROWS - CONS_H)
		{
			cons_y--;
		}
		break;
	case KEY_DOWN:
		if (cons_y < ALL_ROWS)
		{
			cons_y++;
		}
		break;
	case BACKSPACE:
	case DELETE:
		if (cons_x > 0)
		{
			cons_x--;
		}
		else if (cons_y > ALL_ROWS - CONS_H)
		{
			cons_y--;
			cons_x = ALL_COLUMNS - 1;
		}
		break;
	case ENTER:
		if (win != TEXT_CONS) 
		{
			cons_y = 0;
			cons_x = 0;
		}
		break;
	case ESC:
		break;
	default:
		if ((cons_x < ALL_COLUMNS && cons_y < ALL_ROWS) && (str.size() > 1))
		{
			if (cons_x + 1 < ALL_COLUMNS)
			{
				cons_x++;
			}
			else if (cons_y < ALL_ROWS)
			{
				cons_y++;
				cons_x = 0;
			}
		}
		break;
	}

	if (!(symb == ENTER && win == TEXT_CONS))
		adapter->move_cursor(cons_y, cons_x);
	show(TEXT);
}

void View::update_text(File& file, const int symb, int& x, int& y, const size_t& string_num)
{
	if (symb == ENTER)
	{
		x = 0;
		y = 0;
	}

	if (y >= ALL_ROWS - CONS_H)
	{
		y -= scroll_down(file, string_num);
	}
	if (y < 0)
	{
		y += scroll_up(file, string_num);
	}

	text_block.put_data(file, adapter);
	show(TEXT);
	adapter->move_cursor(y, x);
}

void View::update_cursor(int x, int y)
{
	adapter->move_cursor(y, x);
}

void View::update_stat(MyString filename, size_t cur_str, size_t str_am)
{
	if (cur_str <= str_am)
	{
		status_block.put_data(filename, cur_str, str_am, adapter);
	}
}

///////////////////////////////////////////////////////////////////////////////

void View::new_file()
{
	text_block.set_start(0);
}

void View::back_to_text(int& x, int& y)
{
	adapter->move_cursor(y, x);
	show(TEXT);
}

void View::set_new_start(const size_t index, int& x, int& y)
{
	x = 0;
	y = 0;
	adapter->move_cursor(y, x);
	text_block.set_start(index);
	show(TEXT);
}

////////////////////////////////////////////////////////////////////////////
WINDOW* View::Block::create_new_block(const int height, const int width, const int starty, const int startx, TUIInterface* adapter, int n, int color1, int color2)
{
	WINDOW* local_win;
	local_win = adapter->_newwin(height, width, starty, startx);
	adapter->create_color_id(n, color1, color2);
	adapter->_wbkgd(local_win, COLOR_PAIR(n));
	return local_win;
}

////////////////////////////////////////////////////////////////////////////
void View::TextBlock::set_start(const size_t new_start_ind)
{
	text_out_str = new_start_ind;
}

size_t View::TextBlock::get_start()
{
	return text_out_str;
}

void View::TextBlock::show(TUIInterface* adapter)
{
	adapter->_wrefresh(window);
}

View::TextBlock::TextBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter)
{
	text_out_str = 0;
	window = create_new_block(height, width, y, x, adapter, 1, COLOR_WHITE, COLOR_BLACK);
	adapter->_wrefresh(window);
}

void View::TextBlock::put_data(File& file, TUIInterface* adapter)
{
	adapter->_wclear(window);
	int y = 0, x = 0;
	int j;

	for (int i = text_out_str; i < file.number_lines(); i++)
	{
		j = 0;
		if (y < ALL_ROWS - CONS_H)
		{
			while ((y < ALL_ROWS - CONS_H || x < ALL_COLUMNS) && (file.lines[i])[j] != '\0')
			{
				adapter->print(window, y, x, (file.lines[i])[j]);
				if ((file.lines[i])[j] == '\n')
				{
					y++;
					x = 0;
					break;
				}
				else
					x++;

				j++;
				if (x == ALL_COLUMNS)
				{
					y++;
					x = 0;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
View::ConsBlock::ConsBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter)
{
	window = create_new_block(height, width, y, x, adapter, 2, COLOR_BLACK, COLOR_WHITE);
	adapter->_wrefresh(window);
}

void View::ConsBlock::show(TUIInterface* adapter)
{
	adapter->_wrefresh(window);
}

void View::ConsBlock::put_data(MyString& str, TUIInterface* adapter)
{
	adapter->_wclear(window);
	adapter->print(window, 0, 0, str.c_str());
}

////////////////////////////////////////////////////////////////////////////
View::StatusBlock::StatusBlock(const int height, const int width, const int y, const int x, TUIInterface* adapter)
{
	window = create_new_block(CONS_H, MODE_L, y, x, adapter, 3, COLOR_BLACK, COLOR_WHITE);
	filename = create_new_block(CONS_H, NAME_L, y, x + MODE_L, adapter, 3, COLOR_BLACK, COLOR_WHITE);
	count_string = create_new_block(CONS_H, NUMBER_L, y, x + MODE_L + NAME_L, adapter, 3, COLOR_BLACK, COLOR_WHITE);

	adapter->_wrefresh(window);
	adapter->_wrefresh(filename);
	adapter->_wrefresh(count_string);
}

void View::StatusBlock::put_data(MyString& file_name, const size_t cur_str, const size_t str_am, TUIInterface* adapter)
{
	adapter->_wclear(count_string);
	adapter->_wclear(filename);
	adapter->print(count_string, 0, 0, cur_str, str_am);
	adapter->print(filename, 0, 0, file_name.c_str());
}

void View::StatusBlock::put_mode(int& mode, TUIInterface* adapter)
{
	adapter->_wclear(window);
	switch (mode)
	{
	case NORMAL:
		adapter->print(window, 0, 0, " --NORMAL-- ");
		break;
	case COMMAND:
		adapter->print(window, 0, 0, " --COMMAND-- ");
		break;
	case SEARCH:
		adapter->print(window, 0, 0, " --SEARCH-- ");
		break;
	case INSERT:
		adapter->print(window, 0, 0, " --INSERT-- ");
		break;
	case HELP:
		adapter->print(window, 0, 0, " --HELP-- ");
		adapter->_wclear(count_string);
		adapter->_wclear(filename);
		break;
	}
}

void View::StatusBlock::show(TUIInterface* adapter)
{
	adapter->_wrefresh(window);
	adapter->_wrefresh(filename);
	adapter->_wrefresh(count_string);
}