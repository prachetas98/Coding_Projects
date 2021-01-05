/////
// Name: Prachetas Deshpande
// cs 2400
// hw 6
/////
#pragma once
class money
{
public:
	money();
	~money();					// a default constructor
	int getdollars();				
	void setdollars(int d);
	int getcents();					// Function prototypes
	void setcents(int c);
	money add(money,int);
	money subtract(money, int);
private:
	int dollars;					// private variables
	int cents;
};

