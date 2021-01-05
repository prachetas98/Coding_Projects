/////
// Prachetas Deshpande
// cs 2400
// hw 6
/////
#include "money.h"


money::money()								// the default constructor
{
}


money::~money()
{
}

int money::getdollars() {						// function to get the dollars
	return dollars;
}
void money::setdollars(int d) {						// function to set the dollars
	dollars = d;
}
int money::getcents() {							// function to get the cents
	return cents;
}
void money::setcents(int c) {						// function to set the cents
	cents = c;
}

money money::add(money m,int k) {					// function to add the money
	m.dollars = m.dollars + k;
	return m;
}

money money::subtract(money m, int k) {					// function to subtract the money
	m.dollars = m.dollars - k;
	return m;
}
