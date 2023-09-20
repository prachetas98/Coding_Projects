#include "stdafx.h"
#include "MinHeap.h"
#include <iostream>
using namespace std;
MinHeap::MinHeap(int cap)
{
	heap_size = 0;
	capacity = cap;
	harr = new city[cap];
}


MinHeap::~MinHeap()
{
}

void MinHeap::insertKey(city& k)
{
	if (heap_size == capacity)
	{
		cout << "\nOverflow: Could not insertKey\n";
		return;
	}

	// First insert the new key at the end 
	heap_size++;
	int i = heap_size - 1;
	harr[i] = k;

	// Fix the min heap property if it is violated 
	while (i != 0 && harr[parent(i)].dist > harr[i].dist)
	{
		swap(harr[i], harr[parent(i)]);
		i = parent(i);
	}
	cout << "The function finds the minimum heaps" << endl;
}

// Decreases value of key at index 'i' to new_val.  It is assumed that 
// new_val is smaller than harr[i]. 
/*
void MinHeap::decreaseKey(int j, int new_val)
{
	harr[j].dist = new_val;
	while (j != 0 && harr[parent(j)].dist > harr[j].dist)
	{
		swap(harr[j], harr[parent(j)]);
		j = parent(j);
	}
}
*/
// Method to remove minimum element (or root) from min heap 
city MinHeap::deleteKey()
{
	//if (heap_size <= 0)
		//return INT_MAX;
	if (heap_size == 1)
	{
		heap_size--;
		return harr[0];
	}

	// Store the minimum value, and remove it from heap 
	city root = harr[0];
	harr[0] = harr[heap_size - 1];
	heap_size--;
	MinHeapify(0);

	return root;
}


// This function deletes key at index i. It first reduced value to minus 
// infinite, then calls extractMin() 
/*
void MinHeap::deleteKey(int i)
{
	decreaseKey(i, INT_MIN);
	extractMin();
}
*/
// A recursive method to heapify a subtree with the root at given index 
// This method assumes that the subtrees are already heapified 
void MinHeap::MinHeapify(int i)
{
	int l = left(i);
	int r = right(i);
	int smallest = i;
	if (l < heap_size && harr[1].dist < harr[i].dist)
		smallest = l;
	if (r < heap_size && harr[r].dist < harr[smallest].dist)
		smallest = r;
	if (smallest != i)
	{
		swap(harr[i], harr[smallest]);
		MinHeapify(smallest);
	}
}

void swap(city &x, city &y)
{
	city temp;
	temp = x;
	x = y;
	y = temp;
}
