#pragma once
#include <string>
#include <vector>
using namespace std;

class city {
public:
	//string name;
	int i;
	int dist;
	city() {}
	~city() {}
	city(const city &c) { i = c.i; dist = c.dist; }
	city(int l, int distance) { i = l; dist = distance; }
};

void swap(city &x, city &y);
class MinHeap
{
	//vector<city> c;
	city* harr;
	int capacity; // maximum possible size of min heap 
	int heap_size;
public:
	MinHeap(int cap);
	city deleteKey();

	//void decreaseKey(int i, int new_val);

	int getMin() { return harr[0].i; }

	//void deleteKey(int i);
 
	void insertKey(city& k);

	void MinHeapify(int i);

	int parent(int i) { return (i - 1) / 2; }

	int left(int i) { return (2 * i + 1); }

	int right(int i) { return (2 * i + 2); }

	~MinHeap();

};

