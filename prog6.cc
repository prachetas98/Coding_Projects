/////
// Name: Prachetas Deshpande
// cs 2400
// homework6
/////


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "door.h"								// header file for door
#include "room.h"								// header file for room
#include "player.h"								// header file for player
#include "money.h"								// header file for money
using namespace std;

void getting_line(ifstream&, vector<room>&);
void openfile(ifstream&);
void setupPlayer(player&);
void start_game(vector<room>&, player&);
int go_game(vector<room>&, player&, int);
void pick_up(room&, player&,int);
void listing_inventory(player&);
void locking(room&, player&);
bool unlocking(room&, player&);
int main()
{

	char a;
	player p;
	vector<room> r1;
	string line;
	ifstream in_file;


	cout << "hit'p' to play or 'q' to quit" << endl;			// to play the game or quit
	cin >> a;
	if (a == 'p') {
		openfile(in_file);						// opening the input file
		
		getting_line(in_file,r1);					// getting the input file line
		setupPlayer(p);							// choosing the player
		start_game(r1, p);						// start to play the game
		
	}
	in_file.close();							// closing the input file
	return 0;
}

void openfile(ifstream& in_file) {
	char in_file_name[80];
	cout << "Give the name of the input file" << endl;
	cin >> in_file_name;
	in_file.open(in_file_name);						// function to open the file
	if (in_file.fail()) {
		cout << "Input error" << endl;					// if it fails to open
	}
}

void getting_line(ifstream& in_file, vector<room>& r1) {			// function to get the line from the file
	string line,str1,str2;
	int index;
	int i = 0;
	
	
	bool isParse = false;						
	while (getline(in_file, line)) {
		
		if (index = line.find("//") < line.length()) {			// finding the "//" line
			isParse = true;
		}
		if ((isParse==false) && (index = line.find("#") < line.length())) {	// finding the "#" sign
			isParse = true;
			str1 = line;
			//r.SetDescription(line);
		}
		if (isParse == false) {
			str2 = line;
			//r.SetCharacter(line);
		}
		if (!str1.empty() && !str2.empty()) {
			room r;
			r.SetDescription(str1);			// calling the function from implementation file
			r.SetCharacter(str2);			// calling the function from implementation file		
			r.Set_inventory(r1.size());		// calling the function from the implementation file
			str1 = "";
			str2 = "";
			r1.push_back(r);
		}
		
			isParse = false;
		
	}

	//cout << "my size is" << r1.size() << endl;
}

void setupPlayer(player& p) {
	char b;
	bool bw=false;
	string str;
	cout << "Give the name of the character: " << endl;
	cout << "b for batman s for spiderman k for kirby m for superman h for hulk" << endl;
	while (bw == false) {
		cin >> b;
		switch (b) {
		case 'b':
			str = "batman";						// choose your favourite player
			bw = true;
			break;
		case 's':
			str = "spiderman";
			bw = true;
			break;
		case 'k':
			str = "kirby";
			bw = true;
			break;
		case 'm':
			str = "superman";
			bw = true;
			break;
		case 'h':
			str = "hulk";
			bw = true;
			break;
		default:
			str = "wrong symbol entered";			// if the wrong symbol is entered
		}
	}
	p.setAvatar(str);
}

void start_game(vector<room>& r2, player& p1) {
	char c;
	bool bquit = false;
	int current_room = 0;
	cout<<r2[current_room].get_description()<<endl;
	while (bquit==false) {						// The menu to play the game
		cout << "G for go" << endl;
		cout << "P for pick up" << endl;
		cout << "I for list inventory" << endl;
		cout << "D to describe the current room" << endl;
		cout << "try E to exit the game" << endl;
		cout << "L for lock" << endl;
		cout << " U for unlock" << endl;
		cin >> c;
		switch (c) {
		case 'G':
			current_room=go_game(r2,p1,current_room);	// using the 'room' object and the 'player' object
			break;
		case 'P':
			pick_up(r2[current_room],p1,current_room);	// function to execute the pick up option
			break;
		case 'I':
			listing_inventory(p1);				// function to execute the list inventory option
			break;
		case 'D':
			cout << r2[current_room].get_description() << endl;
			break;
		case 'E':
			if (r2[current_room].get_exit() == true) {
				bquit = true;
			}
			break;
		case 'L':
			locking(r2[current_room],p1);
			break;
		case 'U':
			unlocking(r2[current_room], p1);
			break;
		default:
			cout << "wrong option entered" << endl;

		}
	}
}

int go_game(vector<room>& r2, player& p1, int n)			// function for the go option
{
	bool bfound=false;
	bool unlock = true;
	char A,U;
	string str1,str2;
	unsigned int i=0;
	int current_door;
	
	cout << "give the direction you want to go" << endl;
	cout << "N for North ,S for South, E for East, W for West" << endl;
	cout << "V for north-west, X for north-east, Y south-west, Z-south-east" << endl;
	cin >> A;
	switch (A) {							// statement to find the directions
	case 'N':
		str1 = "North";
		break;
	case 'S':
		str1 = "South";
		break;
	case 'E':
		str1 = "East";
		break;
	case 'W':
		str1 = "West";
		break;
	case 'V':
		str1 = "North-West";
		break;
	case 'X':
		str1 = "North-East";
		break;
	case 'Y':
		str1 = "South-West";
		break;
	case 'Z':
		str1 = "South-East";
		break;
	default:
		cout << "Incorrect option entered" << endl;
	}
	
	while ((i < r2[n].get_door().size()) && (bfound == false)) {
		str2 = r2[n].get_door()[i].get_direction();		// a loop to find and get the door and direction
		if (str1 == str2) {
			current_door = i;
			bfound = true;
		}
		
		i++;
	}
	if (bfound == true) {
		bool lock=r2[n].get_door()[current_door].get_lock();
		if (lock == true) {
			cout << "The door is locked" << endl;
			cout << "Do you want to unlock it?" << endl;	// an option to unlock the door
			cout << "Hit U to unlock" << endl;
			cin >> U;
			if (U == 'U') {
				unlock=unlocking(r2[n], p1);		// a boolean statement
				if (unlock == true) {
					n = r2[n].get_door()[current_door].get_no_of_room();
					cout << "You have entered the other room" << endl;
					return n;			// returning the integer
				}
				else if (unlock == false) {
					cout << "There are no keys" << endl;
				}
				
			}
		}
		else if (lock == false) {
			n = r2[n].get_door()[current_door].get_no_of_room(); // getting the room
			cout << "You have entered the other room" << endl;
			return n;
		}
		
		
	}
	else if (bfound == false) {
		cout << "there is no door in this direction" << endl;     // finding out if ther is a door
	}
	return n;
}

void pick_up(room& r, player& p1, int i) {				// function for the pick-up choice
	char C;
	money amount, amount1;
	int k, k1;
	vector<string> str1;
	string str;
	cout << "give the item you want to pick up" << endl;
	cout << "M for money, K for Keys, I for item" << endl;
	cin >> C;
	switch (C) {							// statement to finf the money,keys and item
	case 'M':
		amount =r.get_money();

		if (amount.getdollars() >= 5) {
			amount = amount.subtract(amount,5);
			amount1 = p1.getMoney();
			amount1 = amount1.add(amount1,5);
			p1.setMoney(amount1);
			r.set_money(amount);
		}
		else {
			cout << "There is not enough money" << endl;
		}
		break;
	case 'K':
		k = r.get_keys();
		if (k > 1) {
			k = k - 1;
			k1 = p1.getKeys();
			k1 = k1 + 1;
			p1.setKeys(k1);
			r.set_keys(k);
		}
		else {
			cout << "There is no key" << endl;
		}
		break;
	case 'I':
		str = r.get_inventory();
		if (str.empty()) {
			cout << "There are no items in the room" << endl;
		}
		else {
			p1.setInventory(i);
			r.Set_inventory(6);

		}
		break;

	}
}

void listing_inventory(player& p1) {					// function to list the inventory
	int j;
	string str="";
	for (unsigned int i = 0; i < p1.get_inventory().size(); i++) {
		j = p1.get_inventory()[i];
		switch (j) {						// statement to choose power of the chracter
		case 0:
			str+="batmobile";
			break;
		case 1:
			str+= " web";
			break;
		case 2:
			str+= " hammer";
			break;
		case 3:
			str+= " crystal";
			break;
		case 4:
			str+= " magic sword";
			break;
		default:
			str+= "";
		}
		
	}
	cout <<"You have the following items "<< str << endl;
	cout << "You have $ " << p1.getMoney().getdollars() <<"."<<p1.getMoney().getcents()<< endl;
	cout << "You have " << p1.getKeys()<<" keys" << endl;
	
}

void locking(room& r2, player& p1) {					// a function to lock
	int i = p1.getKeys();
	if (i > 0) {
		i--;
		p1.setKeys(i);

	}
}

bool unlocking(room& r2, player& p1) {					// a function to unlock
	bool b=false;
	int i = p1.getKeys();
	if (i > 0) {
		i--;
		p1.setKeys(i);
		b = true;
	}
	return b;
}





