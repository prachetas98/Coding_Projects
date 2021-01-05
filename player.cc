/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////
#include "player.h"
#include <iostream>
using namespace std;


player::player()							// a default constructor for players
{
	money1.setdollars(0);
	money1.setcents(0);
	keys = 0;
	
}


player::~player()
{
}

string player::getAvatar(){					// function to get the player
	return avatar;
}
int player::getKeys(){						// function to get the key
	return keys;
}
vector<int> player:: get_inventory(){				// function to get the inventory
	return inventory;
}
money player:: getMoney(){					// function to get the money
	return money1;
}
void player:: setAvatar(string str){				// function to set the player
	avatar = str;
}
void player::setKeys(int k){					// function to set the keys
	keys = k;
}
void player::setInventory(int i){				// function to set the inventory
	inventory.push_back(i);		// each item will have number
}
void player::setMoney(money mon){				// function to set the money
	money1 = mon;
}

