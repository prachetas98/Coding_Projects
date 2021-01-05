/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "money.h"
using namespace std;
class player
{
public:
	player();							// a default constructor
	~player();
	string getAvatar();
	int getKeys();
	vector<int> get_inventory();
	money getMoney();
	void setAvatar(string str);					// the function prototypes
	void setKeys(int k);
	void setInventory(int i);
	void setMoney(money mon);

private:
	string avatar;
	int keys;							// private variables
	vector<int> inventory;						// a private vector
	money money1;

};

