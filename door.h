/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////
#pragma once
#include <iostream>
#include <fstream>
using namespace std;
class door
{
public:
	door();							// Default constructor
	~door();
	door(string str1, int number, bool b);
	friend ostream& operator <<(ostream& outs, door& d);	// friend function
	int get_no_of_room();
	string get_direction();					// function prototypes
	bool get_lock();
private:
	int no_of_room;
	string directions;					// private variables
	bool lock;
};

