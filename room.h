/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////
#pragma once
#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
#include <string>
#include "door.h"
#include "money.h"
using namespace std;
class room
{
public:
	room();								// the default constructor
	~room();
	room(int k, double d, vector<door> v, bool b);
	void SetDescription(string line);
	void SetCharacter(string line);
	int get_keys();							// function prototypes
	money get_money();
	vector<door> get_door();					// vector function prototype
	bool get_exit();
	string get_description();
	string get_inventory();
	void Set_inventory(int);
	void set_money(money);
	void set_keys(int);

private:
	int keys;
	money money1;							// private variables
	vector<door> v1;						// private variable vector
	bool bexit;
	string description;
	string inventory;
}; 

#endif 
