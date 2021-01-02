/////
// Name: Prachetas Deshpande
// CS 3610
// Project2
/////
// TIME COMPLEXITY:
//O(nlogn) where n is the number of unique characters. If there are n nodes, extractMin() is called 2*(n – 1) times. extractMin() takes O(logn) time as it calles minHeapify(). So, overall complexity is O(nlogn). Without the min heap with linear search it will take O(n) time and since there are 'n' nodes present. The overall time complexity would become O(n^2).
/////

#include <iostream>
#include <iomanip>
#include "huffman_tree.h"
#include <string>
using namespace std;

int main(int argc, char** argv) {
	
	int count;
	HuffmanTree tree;
	cin >> count;
	cin.ignore();
	for (int i = 1; i <= count; i++) {
		string str;
		getline(cin, str);
		tree.construct(str);
		cout << "Test Case: " << i << "\n";
		tree.print();
		tree.destruct();
		cout << endl;
		
	}
	return 0;
	
	
}


