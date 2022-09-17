#include "File.h"

File::File(MyString& path)
{
    if (path.empty())
    {
        this->name = sample_path;
        create();
    }
    else name = path;

    modify = false;

    сur_page = 0;
    num_start_page.push_back(0);
}

bool File::load()
{
    ifstream file_input(name.c_str());
    if (!file_input) return false;

    lines.clear();
    сur_page = 0;
    num_start_page.clear();
    num_start_page.push_back(0);

    MyString line;
    do
    {
        file_input >> line;
        line.append(1, '\n');
        lines.push_back(line);
    } while (!file_input.eof());

    file_input.close();

    if (lines.empty())
    {
        MyString empty;
        lines.push_back(empty);
    }

    сur_page = 0;
    num_start_page.push_back(0);

    return true;
}

bool File::save(MyString& filename)
{
    modify = false;
    ofstream file_output(filename.c_str());
    if (!file_output) return false;

    for (MyString& line : lines)
    {
        file_output << line;
    }

    file_output.close();
    modify = false;
    return true;
}

void File::create()
{
    MyString empty_line;
    lines.push_back(empty_line);

    ofstream file(name.c_str());
    file.close();
}

File& File::operator=(const File& other)
{
    this->modify = other.modify;
    this->name = other.name;
    this->lines = other.lines;

    return*this;
}

void File::search(MyString& substr, int& x, int& y, bool is_direct)
{
    if (substr.empty())
    {
        return;
    }

    if (is_direct) 
    {
        direct_search(substr, x, y);
    }
    else 
    {
        reverse_search(substr, x, y);
    }
}

void File::direct_search(MyString& substr, int& x, int& y)
{
    auto iter = this->begin();
    advance(iter, y); //переходим к текущей строке

    int pos;
    MyString line = *iter;

    //сначала ищем в той же строке с позиции x
    if (line.size() >= substr.size())
    {
        pos = line.find(substr.c_str(), x + 1);
        if (pos != -1)
        {
            x = pos;
            return;
        }
    }

    ++iter;
    for (int num_line = y + 1; iter != this->end(); ++num_line, ++iter)
    {
        line = *iter;
        if (substr.size() > line.size()) continue;

        pos = line.find(substr.c_str());
        if (pos != -1)
        {
            x = pos;
            y = num_line;
            return;
        }
    }
}

void File::reverse_search(MyString& substr, int& x, int& y)
{
    if (y == 0 && x == 0) return;

    auto iter = this->begin();
    advance(iter, y);

    int pos = -1;
    MyString line = *iter;

    if (line.size() >= substr.size())
    {
        int last_pos;
        do {
            last_pos = pos;
            pos = line.find(substr.c_str(), last_pos + 1);
        } while (pos != -1 && pos < x);

        pos = last_pos;
        if (pos  != -1 && pos < x)
        {
            x = pos;
            return;
        }
    }

    if (iter == this->begin()) return;

    --iter;
    for (int num_line = y - 1; num_line > 0; --num_line, --iter)
    {
        line = *iter;

        if (substr.size() > line.size()) continue;

        int last_pos;
        do {
            last_pos = pos;
            pos = line.find(substr.c_str(), last_pos + 1);
        } while (pos != -1);

        pos = last_pos;
        if (pos != -1)
        {
            x = pos;
            y = num_line;
            return;
        }
    }
}