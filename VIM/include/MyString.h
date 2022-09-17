#ifndef __MYSTRING_H__
#define __MYSTRING_H__
#include<stdio.h>
#include<iostream>
#include<new>

using namespace std;

class MyString
{
public:
	MyString(); // default constructor
	MyString(const MyString& str); //copy constructor
	MyString(const char* str); // char array constructor
	MyString(const initializer_list<char>& list); // initializer list constructor 
	MyString(string& str); // std::string constructor
	MyString(const char* s, size_t count);// init class with count characters of  “char string”,Constructor(“hello”, 4) -> “hell”
	MyString(size_t count, char c); // init class with count of characters Constructor(4, ‘a’)->“aaa”
	MyString(const MyString& str, size_t index, size_t count); // use in function find()

	~MyString(); // destructor

	//operator

	MyString operator+ (const MyString& str); // concatenate with Mystring
	MyString operator+ (const char* str); // concatenate with char array
	MyString operator+ (string& str); // concatenate with std::string
	MyString operator+ (char c); // concatenate with char

	MyString operator+= (const MyString& str);// assignment concatenate constructor
	MyString operator+= (const char* s); // assignment concatenate with char array
	MyString operator+= (char c);// char assignment
	MyString operator+= (string& str); 	// concatenate with std::string

	MyString& operator= (const MyString& str); // constructor string assignment
	MyString& operator= (const char* str); // char string assignment
	MyString& operator= (string& str); // std::string assignment
	MyString& operator= (char c); // char assignment
	char operator[] (int i); // index operator, Mystring(“012345”)[5] ->‘5’

	// lexicographically comparing

	bool operator>(const MyString& str);
	bool operator> (const char* str);
	bool operator> (string& str);

	bool operator<(const MyString& str);
	bool operator<  (const char* str);
	bool operator< (string& str);

	bool operator>=(const MyString& str);
	bool operator>= (const char* str);
	bool operator>= (string& str);

	bool operator<=(const MyString& str);
	bool operator<= (const char* str);
	bool operator<= (string& str);

	bool operator!=(const MyString& str);
	bool operator!= (const char* str);
	bool operator!= (string& str);

	bool operator==(const MyString& str);
	bool operator== (const char* str);
	bool operator== (string& str);

	////////////////////////////////////////////////////////

	char* c_str(); // return a pointer to null-terminated character array
	char* data(); // return a pointer to array data that not required to be null-terminated
	int length(); // same as size 
	int size(); // return the number of char elements in string
	bool empty(); // true if st getlinering is empty
	int capacity(); // return the current amount of allocated memory for array
	void shrink_to_fit(); //reduce the capacity to size
	void clear(); // remove all char element in string

	int change_capacity();
	void copy_str(char* str1, const char* str2, size_t size);
	size_t size_char(const char* str);

	friend ostream& operator<<(ostream&, MyString& str);
	friend istream& operator>>(istream&, MyString& str);

	////////////////////////////////////////////////////////

	MyString& insert(size_t index, size_t count, char c); // insert count of char in index position
	MyString& insert(size_t index, const char* str); // insert null-terminated char string at index position
	MyString& insert(size_t index, const char* str, size_t count); // insert count of null-terminated char string at index position
	MyString& insert(size_t index, string str); // insert std::string at index position
	MyString& insert(size_t index, string str, size_t count);// insert count of std::string at index position
	MyString& insert(size_t index, const MyString& str);

	////////////////////////////////////////////////////////

	MyString& erase(size_t index, size_t count);

	////////////////////////////////////////////////////////

	MyString& append(const MyString& str);
	MyString& append(const MyString& str, size_t index, size_t count);
	MyString& append(size_t n, char c); // append count of char
	MyString& append(const char* str); // append null-terminated char string
	MyString& append(const char* str, size_t index, size_t count);
	MyString& append(string& str); // append std:: string
	MyString& append(string& str, size_t index, size_t count); // append a count of std:: string by index position

	////////////////////////////////////////////////////////

	MyString& replace(size_t index, size_t count, const MyString& str);
	MyString& replace(size_t index, size_t count, const char* s); // replace a count of char at index by “string”
	MyString& replace(size_t index, size_t count, string str);

	////////////////////////////////////////////////////////

	MyString substr(size_t index); //return a substring starts with index position
	MyString substr(size_t index, size_t count); // return a count of substring’s char starts with index position

	////////////////////////////////////////////////////////

	int find(const MyString& str);
	int find(const MyString& str, size_t index);
	int find(const char* str); // if founded return the index of substring
	int find(const char* str, size_t index); // if founded return the index of substring
	int find(string& str);
	int find(string& str, size_t index); // if founded return the index of substring

private:
	char* _myString; //string
	size_t _size; //size of string
	size_t _capacity; //max size of string
};

#endif // _MYSTRING_H_