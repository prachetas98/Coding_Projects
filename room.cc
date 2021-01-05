/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////

#include <iostream>
#include <math.h>
#include <cstdlib>
#include "room.h"
#include "money.h"


room::room()							// A default constructor
{
}


room::~room()
{
}

room::room(int k, double m, vector<door>v, bool b) {
}

void room::SetDescription(string str) {				// function to set the description
	description = str;
}

void room::SetCharacter(string line) {				// function to set the character
	int i = 0, index, start, end, number;
	
	string str1, str2, str3, str, str4, str5, str6;
	bool b = true;
	bexit= false;
	
	while ((index = line.find("exit ", i)) < line.length()) {
		if ((start = line.find(" ", index)) < line.length() && ((end = line.find(" ", start + 1)) < line.length())) {
			str1 = line.substr(start + 1, end - start - 1);
			//cout << str1 << endl;

		}
		if ((end + 1) < line.length())
			start = end + 1;
		if ((end = line.find(" ", start + 1)) < line.length()) {
			str2 = line.substr(start, end - start);
			number = atoi(str2.c_str());
			//cout << str2 << endl;
			//cout << number << endl;
		}
		if ((end + 1) < line.length())
			start = end + 1;
		if ((end = line.find(" ", start + 1)) < line.length()) {
			str3 = line.substr(start, end - start);
			if (int j = str3.find("un") < str3.length()) {
				b = false;
			}

			//cout << str3 << endl;
			//cout << b << endl;
		}
		i = end + 1;

		door d(str1, number, b);

		b = true;
		v1.push_back(d);
		
	}
	
	if ((index = line.find("keys", i)) < line.length()) {
		if ((start = line.find(" ", index + 1)) < line.length() && ((end = line.find(" ", start + 1)) < line.length())) {
			str4 = line.substr(start + 1, end - start - 1);
			keys = atoi(str4.c_str());
			//cout << str4 << endl;
		}
	}
	if ((index = line.find("money", i)) < line.length()) {
		if ((start = line.find(" ", index + 1)) < line.length() && ((end = line.find(" ", start + 1)) < line.length())) {
			str5 = line.substr(start + 1, end - start - 1);
			//money = atof(str5.c_str());
			int m = str5.find(".");
			string str_dollars = str5.substr(0, m);
			int d = atoi(str_dollars.c_str());
		
			int n = str5.length();
			string str_cents = str5.substr(m + 1, n - m + 1);
			int c = atoi(str_cents.c_str());
			money1.setdollars(d);
			money1.setcents(c);
			//cout << str5 << endl;
		}
	}
	if ((index = line.find("exit_game", i)) < line.length()) {
		 bexit = true;
	}
	
	
}

int room::get_keys(){								// a function to get the keys
	return keys;
}

money room::get_money() {							// a function to get the money
	return money1;
}

vector<door> room::get_door() {							// a function to get the door
	return v1;
}

bool room::get_exit() {								// a function to exit
	return bexit;
}

string room::get_description() {						// a function to get the description
	return description;
}

string room::get_inventory() {							// a function to get the inventory
	return inventory;
}

void room::Set_inventory(int m) {						// a function to set the inventory
	switch (m) {
	case 0:
		inventory = "batmobile";
		break;
	case 1:
		inventory = "web";
		break;
	case 2:
		inventory = "hammer";
		break;
	case 3:
		inventory = "crystal";
		break;
	case 4:
		inventory = "magic sword";
		break;
	default:
		inventory = "";
	}
}

void room::set_money(money i) {							// a function to set the money
	money1 = i;
}

void room::set_keys(int j) {							// a function to set the keys
	keys = j;
}
