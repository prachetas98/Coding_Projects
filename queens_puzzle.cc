// queens_puzzle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "nQueensPuzzle.h"
using namespace std;

int main()
{
	nQueensPuzzle queens(10);

	queens.queensConfiguration(0);
	cout << "Number of Solutions: " << queens.solutionsCount() << endl;
    return 0;
}

