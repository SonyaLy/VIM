#include "Model.h"

Model::Model()
{
	mode = NORMAL;
	cons_index = 0;

	buttons.clear();
	scroll_flag = false;
	
	file = File(empty);
	file.load();
	line_under = *file.begin();
	line_under_num = 0;
	strings_amount = file.number_lines();

	x_absolute = 0, y_absolute = 0, x_real = 0, y_real = 0;
}

/////////////////////////////////////////////////////////

bool  Model::is_command(MyString& command) 
{
	if (command.substr(0, 3) == ":o ")
	{
		MyString last_name(file.name);
		file = File();
		file.name = command.substr(3);
		if (file.load() == false)
		{
			file.name = last_name;
			return false;
		}
		line_under = *file.begin();
		line_under_num = 0;
		strings_amount = file.number_lines();
		
		x_absolute = 0, y_absolute = 0, x_real = 0, y_real = 0;
		notify_new_file();
		mode = NORMAL;
		notify_update_mode(mode);
		return true;
	}
	else if (command == ":w" || command == ":wq!" || command == ":x")
	{
		file.save(file.name);
		
		if (command == ":x" || command == ":wq!")
		{
			endwin();
			exit(0);
		}
		return true;
	}
	else if (command.substr(0, 3) == ":w ")
	{
		MyString name(command.substr(3).c_str());
		file.save(name);
		return true;
	}
	else if (command == ":q")
	{
		if (file.modify == false)
		{
			endwin();
			exit(0);
		}
		else
		{
			cons_data.clear();
			cons_data.append("If modifyed, use q!");
			notify_update_com(cons_data, 1, 1, CONS);
			getch();
		}
			return true;
	}
	else if (command == ":q!")
	{
		endwin();
		exit(0);
	}
	else if (command[1] <= '9' && command[1] >= '0')
	{
		MyString num(command.substr(1));
		mode = NORMAL;
		move_str_by_num(num);
		mode = COMMAND;
		return true;
	}
	else if (command == ":h")
	{
		int x_help = 0, y_help = 0;
		buffer_help.lines.clear();
		help();
		
		mode = HELP;
		notify_update_mode(mode);
		notify_update_text(buffer_help, 0, x_help, y_help, 0);
		return true;
	}
	return false;
}

void  Model::move_str_start()
{
	while (x_absolute > 0)
	{
		recognize_symb(KEY_LEFT);
	}
}

void  Model::move_str_end()
{
	if (file.lines[line_under_num].size() == 1) return;
	while (x_absolute != file.lines[line_under_num].size() - 2)
	{
		recognize_symb(KEY_RIGHT);
	}
}

//вызывается из режима Навигация (NG) и Команд (number)
void Model::move_str_by_num(MyString& buttons)
{
	int num = atoi(buttons.c_str());
	if (num > line_under_num)
	{
		size_t prev = line_under_num + 1;
		while (line_under_num + 1 < num && line_under_num != prev)
		{
			prev = line_under_num;
			line_under = file.lines[line_under_num];
			recognize_symb(KEY_DOWN);
		}
	}
	else
	{
		size_t prev = line_under_num + 1;
		while (line_under_num + 1 > num && line_under_num != prev)
		{
			prev = line_under_num;
			line_under = file.lines[line_under_num];
			recognize_symb(KEY_UP);
		}
	}
	move_str_start();
}

void Model::page_down()
{
	if (line_under_num != file.number_lines() - 1)
	{
		scroll_flag = false;
		while (scroll_flag != true && line_under_num != file.number_lines() - 1)
		{
			recognize_symb(KEY_DOWN);
		}

		file.сur_page++;
		file.num_start_page.emplace(file.num_start_page.begin() + file.сur_page, line_under_num);
		notify_set_new_start(line_under_num, x_real, y_real);
		scroll_flag = false;
	}
}

void Model::page_up()
{
	if (file.сur_page != 0)
	{
		file.сur_page--;
		while (line_under_num != file.num_start_page[file.сur_page])
		{
			recognize_symb(KEY_UP);
		}
		notify_set_new_start(file.num_start_page[file.сur_page], x_real, y_real);
		scroll_flag = false;
	}
}

void Model::move_to_word(const MyString& substr)
{
	mode = NORMAL;
	if (prev_search)
	{
		file.search(const_cast<MyString&>(substr), x_search, y_search, prev_search);
		if (y_absolute == y_search && x_absolute == x_search)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		move_str_start();
		while (y_absolute != y_search) recognize_symb(KEY_DOWN);
		while (x_absolute != x_search) recognize_symb(KEY_RIGHT);
	}
	else
	{
		file.search(const_cast<MyString&>(substr), x_search, y_search, prev_search);
		if (y_absolute == y_search && x_absolute == x_search)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		move_str_start();
		while (y_absolute != y_search) recognize_symb(KEY_UP);
		while (x_absolute != x_search) recognize_symb(KEY_RIGHT);
	}
	mode = SEARCH;
	notify_update_stat(file.name, line_under_num + 1, strings_amount);
}

void Model::help()
{
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("");
	buffer_help.lines.push_back("                                             Text Editor by Lyakhova S.\n");
	buffer_help.lines.push_back("                                   -------------------------------------------------\n");
	buffer_help.lines.push_back("                                   ^, 0 / $ - move cursor to beginning / end of line\n");
	buffer_help.lines.push_back("                                   i, I/A - Insert Mode\n");
	buffer_help.lines.push_back("                                   y - Copy cur string\n");
	buffer_help.lines.push_back("                                   p - Paste\n");
	buffer_help.lines.push_back("                                   yw - Copy cur word\n");
	buffer_help.lines.push_back("                                   /text - Find word in direct way\n");
	buffer_help.lines.push_back("                                   ?text - Find word in reverse way\n");
	buffer_help.lines.push_back("                                   dd - Cut cur string\n");
	buffer_help.lines.push_back("                                   :o filename - open file\n");
	buffer_help.lines.push_back("                                   w - Write to current file\n");
	buffer_help.lines.push_back("                                   wq! - Save file and quit\n");
	buffer_help.lines.push_back("                                   q! - Quit without saving\n");
	buffer_help.lines.push_back("                                   -------------------------------------------------\n");
	buffer_help.lines.push_back("                                                 Press ESC to return\n");
}
///////////////////////////////////////////////////////

bool Model::move_left()
{
	if (x_real > 0)
	{
		x_absolute--, x_real--;
	}
	else if (x_absolute > 0)
	{
		x_absolute--;
		x_real = ALL_COLUMNS;
		y_real--;
	}
	else return true;

	return false;
}

bool Model::move_right()
{
	if (x_absolute < line_under.size() - 2)
	{
		x_absolute++;
		x_real++;
		if (x_real >= ALL_COLUMNS)
		{
			x_real = 0;
			y_real++;
		}
	}
	else return true;

	return false;
}

bool Model::move_up()
{
	int last;
	if (y_real <= 0)
	{
		if (y_absolute > 0)
		{
			//scroll_flag = true;
			//return;
		}
		else return true;
	}
	else if (y_absolute <= 0) return true;

	y_real = y_real - x_absolute / ALL_COLUMNS;

	last = line_under.size() - 2;
	y_absolute--;
	line_under_num--;
	line_under = file.lines[line_under_num];

	if (line_under.size() == 1)
	{
		y_real--;
		x_absolute = x_real = 0;
		return false;
	}//когда строка пустая
	if (x_absolute > line_under.size() - 2)
	{
		x_real = (line_under.size() - 2) % ALL_COLUMNS;
		x_absolute = line_under.size() - 2;
		y_real--;
	}
	else
	{
		y_real = y_real - ((line_under.size() - 2) / ALL_COLUMNS + 1 - x_absolute / ALL_COLUMNS);
	}

	return false;
}

bool  Model::move_down()
{
	if (y_real >= ALL_ROWS - CONS_H - 1)
	{
		if (y_absolute < file.number_lines() - 1)
		{
			scroll_flag = true;
		}
		else return true;
	}
	else if (y_absolute == file.number_lines() - 1)
	{
		return true;
	}

	y_real = y_real + (line_under.size() - 2) / ALL_COLUMNS - x_absolute / ALL_COLUMNS;
	y_absolute++;
	line_under_num++;
	line_under = file.lines[line_under_num];

	if (line_under.size() == 1)
	{
		y_real++;
		x_absolute = x_real = 0;
		return false;
	}//когда строка пустая
	if (x_absolute > line_under.size() - 2)
	{
		x_real = (line_under.size() - 2) % ALL_COLUMNS;
		x_absolute = (line_under.size() - 2);
		y_real = y_real + (line_under.size() - 2) / ALL_COLUMNS + 1;
	}
	else
	{
		y_real = y_real + x_absolute / ALL_COLUMNS + 1;
	}

	return false;
}

///////////////////////////////////////////////////////

bool Model::move_left_cons(size_t& index)
{
	if (index > 0)
	{
		index--;
	}
	else return true;
	
	return false;
}

bool Model::move_right_cons(size_t& index, MyString& data)
{
	if (index < data.size())
	{
		index++;
	}
	else return true;
	
	return false;
}

bool Model::move_up_cons(size_t& index, MyString& data)
{
	if ((index > ALL_COLUMNS) && (data.size() > ALL_COLUMNS))
	{
		index -= ALL_COLUMNS;
	}
	else return true;
	
	return false;
}

bool Model::move_down_cons(size_t& index, MyString& data)
{
	if ((data.size() > ALL_COLUMNS) && (data.size() < ALL_COLUMNS * 2) && (index <= data.size() % ALL_COLUMNS))
	{
		index += ALL_COLUMNS;
	}
	else return true;
	
	return false;
}

///////////////////////////////////////////////////////

void Model::recognize_symb(const int symb)
{
	switch (mode)
	{
	case NORMAL:
		recog_normal(symb);
		break;
	case COMMAND:
		recog_command(symb);
		break;
	case SEARCH:
		recog_search(symb);
		break;
	case INSERT:
		recog_insert(symb);
		break;
	case HELP:
		recog_help(symb);
		break;
	}
}

void Model::recog_normal(const int symb)
{
	int prev;
	bool cur = false;
	if (buttons.size() > 0)
	{
		if (buttons == "r")
		{
			if (file.lines[line_under_num].size() == 1)
			{
				if (file.lines.size() == 1)
				{
					buttons.clear();
					return;
				}
				file.lines[line_under_num].erase(x_absolute, 1);
				file.lines[line_under_num].insert(x_absolute, 1, symb);
				file.lines[line_under_num].insert(1, file.lines[line_under_num + 1].c_str(), file.lines[line_under_num + 1].size());
				file.lines.erase(file.lines.begin() + line_under_num + 1);
				line_under = file.lines[line_under_num];
				strings_amount--;
				notify_update_mode(mode);
				notify_update_stat(file.name, line_under_num + 1, strings_amount);
				notify_update_text(file, 0, x_real, y_real, line_under_num);
				buttons.clear();
				return;
			}
			file.lines[line_under_num].erase(x_absolute, 1);
			file.lines[line_under_num].insert(x_absolute, 1, symb);
			line_under = file.lines[line_under_num];
			notify_update_mode(mode);
			notify_update_text(file, 0, x_real, y_real, line_under_num);
			buttons.clear();
			return;
		}
		else if (buttons[buttons.size() - 1] >= '0' && buttons[buttons.size() - 1] <= '9')
		{
			if (symb >= '0' && symb <= '9')
			{
				buttons.append(1, symb);
				return;
			}
			else if (symb == 'G')
			{
				move_str_by_num(buttons);
				buttons.clear();
				return;
			}
			else buttons.clear();
		}
		else
		{
			switch (symb)
			{
			case 'g':
				if (buttons[buttons.size() - 1] == 'g')
				{
					while (line_under_num != 0)
					{
						recognize_symb(KEY_UP);
					}
					move_str_start();
					line_under = file.lines[line_under_num];
					buttons.clear();
					notify_update_stat(file.name, line_under_num + 1, strings_amount);
					notify_update_text(file, symb, x_real, y_real, line_under_num);
				}
				else buttons.clear();

				return;
				break;
			case 'd':
				if (buttons == "d")
				{
					if (file.lines.size() != 1 && line_under_num == file.lines.size() - 1)
					{
						move_up();
						file.lines.erase(file.lines.begin() + line_under_num + 1);
					}
					else 
					{
						file.lines.erase(file.lines.begin() + line_under_num);
						if (file.lines.size() == 0) file.lines.emplace(file.lines.begin() + line_under_num, empty.append(1, '\n')), empty.clear();
						line_under = file.lines[line_under_num];
					}
					strings_amount = file.lines.size();
					move_str_start();
					notify_update_stat(file.name, line_under_num + 1, strings_amount);
					notify_update_text(file, symb, x_real, y_real, line_under_num);
					buttons.clear();
					file.modify = true;
				}
				else buttons.clear();

				return;
				break;
			case 'y':
				if (buttons == "y")
				{
					string_copy.clear();
					string_copy.append(file.lines[line_under_num], 0, file.lines[line_under_num].size() - 1);
					buttons.clear();
				}
				else buttons.clear();

				return;
				break;
			case 'i':
				if (buttons[buttons.size() - 1] == 'd')
				{
					buttons.append(1, symb);
				}
				else buttons.clear();

				return;
				break;
			case 'w':
				if (buttons == "di")
				{
					int i = x_absolute, size = 1, j;
					while (i != 0 && file.lines[line_under_num][i - 1] != ' ')
					{
						i--;
					}
					j = i;
					while (file.lines[line_under_num][j + 1] != ' ' && file.lines[line_under_num][j + 1] != '\n' && file.lines[line_under_num][j + 1] != '\0')
					{
						size++, j++;
					}
					if (file.lines[line_under_num][j + 1] == ' ') size++;
					file.lines[line_under_num].erase(i, size);
					for (int i = 0; i < size; i++)
					{
						recognize_symb(KEY_LEFT);
					}
					line_under = file.lines[line_under_num];

					notify_update_stat(file.name, line_under_num + 1, strings_amount);
					notify_update_text(file, symb, x_real, y_real, line_under_num);
					buttons.clear();
					file.modify = true;
				}
				else if (buttons == "y")
				{
					int i = x_absolute, size = 1, j;
					while (i != 0 && file.lines[line_under_num][i - 1] != ' ')
					{
						i--;
					}
					j = i;
					while (file.lines[line_under_num][j + 1] != ' ' && file.lines[line_under_num][j + 1] != '\n' && file.lines[line_under_num][j + 1] != '\0')
					{
						size++, j++;
					}
					string_copy.clear();
					string_copy.append(file.lines[line_under_num].substr(i, size));
					buttons.clear();
				}
				else buttons.clear();

				return;
				break;
			default:
				buttons.clear();
				return;
				break;
			}
		}
	}

	switch (symb)
	{
	//переход в режим ввода текста
	case 'i':
		mode = INSERT;
		notify_update_mode(mode);
		file.modify = true;
		break;
	case 'I':
		move_str_start();
		mode = INSERT;
		notify_update_mode(mode);
		file.modify = true;
		break;
	case 'A':
		move_str_end();
		mode = INSERT;
		notify_update_mode(mode);
		file.modify = true;
		break;
	case 'S':
		empty.append(1, '\n');
		file.lines.erase(file.lines.begin() + line_under_num);
		file.lines.emplace(file.lines.begin() + line_under_num, empty);
		line_under = file.lines[line_under_num];
		empty.clear();
		move_str_start();
		mode = INSERT;
		notify_update_mode(mode);
		file.modify = true;
		break;
	case 'r':
		mode = INSERT;
		notify_update_mode(mode);
		if (buttons.size() == 0)
		{
			buttons.append(1, symb);
		}
		mode = NORMAL;
		file.modify = true;
		break;
	//переход в режим команд
	case ':':
		mode = COMMAND;
		notify_update_mode(mode);
		cons_index = 1;
		cons_data.clear();
		cons_data.append(1, char(symb));
		notify_update_com(cons_data, symb, cons_index, CONS);
		break;
	case '/':
	case '?':
		mode = SEARCH;
		notify_update_mode(mode);
		search_index = 1;
		search_data.clear();
		search_data.append(1, char(symb));
		notify_update_stat(file.name, line_under_num + 1, strings_amount);
		notify_update_com(search_data, symb, search_index, TEXT_CONS);
		break;
	/////////////////////////
	case 'p':
		if (string_copy.empty()) return;
		file.lines[line_under_num].insert(x_absolute + 1, string_copy);
		line_under = file.lines[line_under_num];
		buttons.clear();
		break;
	case KEY_LEFT:
		if (move_left()) return;
		notify_update_cursor(x_real, y_real);
		return;
		break;
	case KEY_RIGHT:
		if (move_right()) return;
		if (y_real < ALL_ROWS - CONS_H - 1)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
	case KEY_UP:
		cur = false;
		if (y_real > 0) cur = true;
		if (move_up()) return;
		if (cur)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
	case KEY_DOWN:
		if (move_down()) return;
		if (y_real < ALL_ROWS - CONS_H - 1)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
		//переместиться в начало строки
	case '0':
	case '^':
		move_str_start();
		break;
	case '$':
		move_str_end();
		break;
	case 'w':
		if (x_absolute == line_under.size() - 1) return;
		else
		{
			if (file.lines[line_under_num][x_absolute] == ' ') return;
			if (file.lines[line_under_num][x_absolute + 1] == ' ' || file.lines[line_under_num][x_absolute + 1] == '\n')
			{
				do
				{
					recognize_symb(KEY_RIGHT);
				} while (file.lines[line_under_num][x_absolute + 1] != ' ' && file.lines[line_under_num][x_absolute + 1] != '\n');
			}
			else
			{
				while (file.lines[line_under_num][x_absolute + 1] != ' ' && file.lines[line_under_num][x_absolute + 1] != '\n')
				{
					recognize_symb(KEY_RIGHT);
				}
			}
		}
		break;
	case 'b':
		if (x_absolute == 0) return;
		else
		{
			if (file.lines[line_under_num][x_absolute] == ' ') return;
			if (file.lines[line_under_num][x_absolute - 1] == ' ')
			{
				do
				{
					recognize_symb(KEY_LEFT);
				} while (file.lines[line_under_num][x_absolute - 1] != ' ' && x_absolute != 0);
			}
			else
			{
				while (file.lines[line_under_num][x_absolute - 1] != ' ' && x_absolute != 0)
				{
					recognize_symb(KEY_LEFT);
				}
			}
		}
		break;
	case 'G':
		do
		{
			prev = line_under_num;
			recognize_symb(KEY_DOWN);
		} while (line_under_num != prev);
		move_str_end();
		break;
	case KEY_NPAGE:
		page_down();
		break;
	case KEY_PPAGE:
		if (line_under_num == 0) return;
		page_up();
		break;
	case 'x':
		if (file.lines[line_under_num].size() == 0 || (line_under_num == file.lines.size() - 1 && x_absolute == file.lines[line_under_num].size() - 2)
			|| (file.lines[line_under_num].size() == 1 && line_under_num == file.lines.size() - 1)) return;
		if (file.lines[line_under_num][x_absolute + 1] != '\0' && file.lines[line_under_num][x_absolute + 1] != '\n')
		{
			file.lines[line_under_num].erase(x_absolute + 1, 1);
			line_under = file.lines[line_under_num];
		}
		else
		{
			file.lines[line_under_num].erase(file.lines[line_under_num].size() - 1, 1);
			file.lines[line_under_num].insert(file.lines[line_under_num].size(), file.lines[line_under_num + 1].c_str(), file.lines[line_under_num + 1].size());
			file.lines.erase(file.lines.begin() + line_under_num + 1);
			line_under = file.lines[line_under_num];
			strings_amount = file.lines.size();
		}
		break;
	case BACKSPACE:
	case DELETE:
	case ENTER:
		return;
		break;
	default:
		if (buttons.size() == 0)
		{
			buttons.append(1, symb);
		}
		return;
		break;
	}

	notify_update_stat(file.name, line_under_num + 1, strings_amount);
	notify_update_text(file, symb, x_real, y_real, line_under_num);
}

void Model::recog_command(const int symb)
{
	switch (symb)
	{
	case KEY_LEFT:
		if (move_left_cons(cons_index)) return;
		break;
	case KEY_RIGHT:
		if (move_right_cons(cons_index, cons_data)) return;
		break;
	case KEY_UP:
		if (move_up_cons(cons_index, cons_data)) return;
		break;
	case KEY_DOWN:
		if (move_down_cons(cons_index, cons_data)) return;
		break;
	case ENTER:
		if (cons_data.size() > 1 && is_command(cons_data))
		{
			if (cons_data.substr(0, 3) == ":o ")
			{
				notify_update_com(cons_data, symb, cons_index, CONS);
				mode = NORMAL;
				notify_update_mode(mode);
				notify_update_stat(file.name, line_under_num + 1, strings_amount);
				notify_update_text(file, symb, x_real, y_real, line_under_num);
				return;
			}
			else if (cons_data == ":h") return;
			else
			{
				cons_data.clear();
				mode = NORMAL;
				notify_update_mode(mode);
				notify_update_stat(file.name, line_under_num + 1, strings_amount);
				notify_back_to_text(x_real, y_real);
				return;
			}
		}
		else 
		{
			if (cons_data == ":q")
			{
				cons_data.clear();
				cons_data.append("File was modifyed, use q!");
				notify_update_com(cons_data, symb, 1, CONS);
				getch();
			}
			else
			{
				cons_data.clear();
				cons_data.append("Command not executed");
				notify_update_com(cons_data, symb, 1, CONS);
				getch();
			}
			cons_data.clear();
			mode = NORMAL;
			notify_update_mode(mode);
			notify_update_stat(file.name, line_under_num + 1, strings_amount);
			notify_back_to_text(x_real, y_real);
			return;
		}
		break;
	case BACKSPACE:
		if (cons_data.size() > 0)
		{
			if (cons_index == 1 && cons_data.size() > 1) return;
			if (cons_index > 0)
			{
				cons_data.erase(cons_index - 1, 1);
				cons_index--;
			}
		}
		if (cons_data.size() == 0)
		{
			mode = NORMAL;
			notify_update_mode(mode);
			cons_data.clear();
			notify_back_to_text(x_real, y_real);
			return;
		}
		break;
	case ESC:
		break;
	default:
		if (cons_data.size() < ALL_COLUMNS * 2)
		{
			cons_data.insert(cons_index, 1, symb);
			cons_index++;
		}
		break;
	}
	notify_update_com(cons_data, symb, cons_index, CONS);
	return;
}

void Model::recog_search(const int symb) 
{
	if (search_data.size() > 0)
	{
		switch (symb)
		{
		case KEY_LEFT:
			if (move_left_cons(search_index)) return;
			break;
		case KEY_RIGHT:
			if (move_right_cons(search_index, search_data)) return;
			break;
		case KEY_UP:
			if (move_up_cons(search_index, search_data)) return;
			break;
		case KEY_DOWN:
			if (move_down_cons(search_index, search_data)) return;
			break;
		case BACKSPACE:
			if (search_data.size() > 0)
			{
				if (search_index == 1 && search_data.size() > 1)
				{
					return;
				}
				if (search_index > 0)
				{
					search_data.erase(search_index - 1, 1);
					search_index--;
				}
			}
			if (search_data.size() == 0)
			{
				mode = NORMAL;
				notify_update_mode(mode);
				search_data.clear();
				notify_back_to_text(x_real, y_real);
				return;
			}
			break;
		case ESC:
			mode = NORMAL;
			notify_update_mode(mode);
			notify_back_to_text(x_real, y_real);
			return;
			break;
		case ENTER:
			if (search_data.size() <= 1) return;
			x_search = x_absolute, y_search =y_absolute;
			switch (search_data[0])
			{
			case '/':
				prev_search = true;
				move_to_word(search_data.substr(1));
				prev_search_data.clear();
				prev_search_data.append(search_data);
				search_data.clear();
				
				notify_update_mode(mode);
				break;
			case '?':
				prev_search = false;
				move_to_word(search_data.substr(1));
				prev_search_data.clear();
				prev_search_data.append(search_data);
				search_data.clear();
				
				notify_update_mode(mode);
				break;
			}
			break;
		default:
			if (search_data.size() < ALL_COLUMNS * 2)
			{
				search_data.insert(search_index, 1, symb);
				search_index++;
			}
			break;
		}
		notify_update_com(search_data, symb, search_index, TEXT_CONS);
		return;
	}
	else 
	{
		x_search = x_absolute, y_search = y_absolute;
		switch (symb)
		{
		case 'n':
			move_to_word(prev_search_data.substr(1));
			break;
		case 'N':
			prev_search = !prev_search;
			move_to_word(prev_search_data.substr(1));
			break;
		case ESC:
			mode = NORMAL;
			notify_back_to_text(x_real, y_real);
			notify_update_mode(mode);
			//notify_update_stat(file.name, line_under_num + 1, strings_amount);
			notify_update_text(file, symb, x_real, y_real, line_under_num);
			return;
			break;
		}
	}
}

void Model::recog_help(const int symb)
{
	switch (symb)
{
	case ESC:
		mode = NORMAL;
		notify_update_mode(mode);
		notify_back_to_text(x_real, y_real);
		recognize_symb(0);
		return;
		break;
	}
}

void Model::recog_insert(const int symb)
{
	MyString tmp;
	int size;
	bool cur = false;
	switch (symb) 
	{
	case KEY_LEFT:
		if (move_left()) return;
		notify_update_cursor(x_real, y_real);
		return;
		break;
	case KEY_RIGHT:
		if (move_right()) return;
		if (y_real < ALL_ROWS - CONS_H - 1)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
	case KEY_UP:
		cur = false;
		if (y_real > 0) cur = true;
		if (move_up()) return;
		if (cur)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
	case KEY_DOWN:
		if (move_down()) return;
		if (y_real < ALL_ROWS - CONS_H - 1)
		{
			notify_update_cursor(x_real, y_real);
			return;
		}
		break;
	case BACKSPACE:
		if (file.lines[line_under_num].size() > 0)
		{
			if (x_absolute == 0 && line_under_num != 0)
			{
				recognize_symb(KEY_UP);
				move_str_end();
				file.lines[line_under_num].erase(file.lines[line_under_num].size() - 1, 1);
				file.lines[line_under_num].insert(file.lines[line_under_num].size(), file.lines[line_under_num + 1].c_str(), file.lines[line_under_num + 1].size());
				file.lines.erase(file.lines.begin() + line_under_num + 1);
				line_under = file.lines[line_under_num];
				
				strings_amount = file.lines.size();
				notify_update_stat(file.name, line_under_num + 1, strings_amount);
				notify_update_text(file, 0, x_real, y_real, line_under_num);
				return;
			}
			else
			{
				file.lines[line_under_num].erase(x_absolute, 1);
				line_under = file.lines[line_under_num];
				recognize_symb(KEY_LEFT);
			}
		}
		break;
	case ENTER:
		tmp.clear();
		if (x_absolute == 0)
		{
			tmp.append(1, '\n');
			file.lines.emplace(file.lines.begin() + line_under_num, tmp);
			line_under = file.lines[line_under_num];
			recognize_symb(KEY_DOWN);
			move_str_start();
			strings_amount++;
			notify_update_stat(file.name, line_under_num + 1, strings_amount);
			notify_update_text(file, 0, x_real, y_real, line_under_num);
			return;
			break;
		}
		else if (x_absolute == file.lines[line_under_num].size() - 2)
		{
			tmp.append(1, '\n');
			file.lines.emplace(file.lines.begin() + line_under_num + 1, tmp);
			line_under = file.lines[line_under_num];
			recognize_symb(KEY_DOWN);
			move_str_start();
			strings_amount++;
			notify_update_stat(file.name, line_under_num + 1, strings_amount);
			notify_update_text(file, 0, x_real, y_real, line_under_num);
			return;
			break;
		}
		size = file.lines[line_under_num].size();
		file.lines[line_under_num].insert(x_absolute, 1, '\n');
		tmp = file.lines[line_under_num].substr(x_absolute + 1, size - x_absolute);
		file.lines[line_under_num].erase(x_absolute + 1, size - x_absolute);
		file.lines.emplace(file.lines.begin() + line_under_num + 1, tmp);
		line_under = file.lines[line_under_num];
		recognize_symb(KEY_DOWN);
		move_str_start();
		strings_amount++;
		notify_update_stat(file.name, line_under_num + 1, strings_amount);
		notify_update_text(file, 0, x_real, y_real, line_under_num);
		return;
		break;
	case KEY_NPAGE:
		page_down();
		break;
	case KEY_PPAGE:
		if (line_under_num == 0) return;
		page_up();
		break;
	case ESC:
		mode = NORMAL;
		notify_update_mode(mode);
		//return;
		break;
	default:
		if (file.lines[line_under_num].size() == 1)
		{
			file.lines[line_under_num].insert(x_absolute, 1, symb);
			line_under = file.lines[line_under_num];
			recognize_symb(KEY_RIGHT);
			break;
		}
		file.lines[line_under_num].insert(x_absolute, 1, symb);
		line_under = file.lines[line_under_num];
		recognize_symb(KEY_RIGHT);
		break;
	}
	notify_update_stat(file.name, line_under_num + 1, strings_amount);
	notify_update_text(file, symb, x_real, y_real, line_under_num);
	return;
}

///////////////////////////////////////////////////////