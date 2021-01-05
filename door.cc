/////
// Name: Prachetas Deshpande
// cs 2400
// Homework 6
/////
#include "door.h"
#include <iostream>
#include <string>
using namespace std;


door::door()							// The default constructor
{
}


door::~door()
{
}

door::door(string str1, int number, bool b) {			// default constructor with two arguments
	directions = str1;
	no_of_room = number;
	lock = b;

}

ostream& operator <<(ostream& outs, door& d) {			// the friend overloaded operator
	outs << "The direction is" << d.directions << " and the room is " << d.no_of_room << " and it is" << d.lock << endl;
	return outs;
}

int door::get_no_of_room() {					// to get the room number
	return no_of_room;
}

string door::get_direction() {					// to get the direction
	return directions;
}

bool door::get_lock() {						// to find out if the door is locked or not
	return lock;
}
