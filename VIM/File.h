#ifndef __FILE_H__
#define __FILE_H__

#include <fstream>
#include <vector>
#include <MyString.h>

using namespace std;

class File
{
public:
    File() = default;
    File(MyString & path);

    using StringList = vector<MyString>;
    StringList lines;
    MyString name;
    bool modify;
    const MyString sample_path = MyString("__SAMPLE__.txt");

    int ñur_page;
    vector<int> num_start_page;

    int number_lines() const
    {
        return lines.size();
    }

    StringList::iterator begin()
    {
        return lines.begin();
    }

    StringList::iterator end()
    {
        return lines.end();
    }

    void push_back(MyString& line)
    {
        lines.push_back(line);
    }

    File& operator= (const File& other);

    bool load();
    bool save(MyString& filename);
    void create();
    void search(MyString& substr, int& x, int& y, bool is_direct);
private:
    void direct_search(MyString& substr, int& x, int& y);
    void reverse_search(MyString& substr, int& x, int& y);
};

#endif //__FILE_H__