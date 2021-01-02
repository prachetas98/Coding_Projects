/////
//Name: Prachetas Deshpande
// Project 2
// CS 3610
/////
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

template <typename T>
struct HeapNode {
	HeapNode(const T data, const int key) : data(data), key(key) {}
	HeapNode(const HeapNode& copy_node) { data = copy_node.data; key = copy_node.key; }
	bool operator<(const HeapNode<T>& rhs) { return this->key < rhs.key; }
	bool operator<=(const HeapNode<T>& rhs) { return this->key <= rhs.key; }
	HeapNode<T> operator=(const HeapNode<T>& rhs) { data = rhs.data; key = rhs.key; return rhs; }
	T data;
	int key;
};

template <typename T>
class MinHeap {
public:
	MinHeap() {}					// the dafault constructor of min heap
	~MinHeap() {}					// the default destructor of min heap

	void insert(const T data, const int key);	// function to insert a node into the tree
	T extract_min();				// function to extract the node with minimum frequency
	T peek() const;					// function to obtain the data of the node
	void MinHeapify(const int idx);			// function to make sure that the parent node is less than its children
	int size() const { return heap.size(); }        // function to return the size of the heap

private:
	vector<HeapNode<T> > heap;
};


// this function is used to insert a node into the tree
template <typename T>
void MinHeap<T>::insert(const T data, const int key) { 
	// First insert the new key at the end
	int i = heap.size();
	HeapNode<T>* new_node = new HeapNode<T>(data, key);

	heap.push_back(*new_node); 				// pushes the new node into the vector
	if (heap.size() > 1) {
		while (i != 0 && new_node->key < heap[(i - 1) / 2].key)
		{

			HeapNode<T> temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;		// this rearranges the tree by switching the position of the nodes
			i = (i - 1) / 2;
			

		}
		
	}

} 
	
// This function is used to find the minimum node in the tree
template <typename T>
T MinHeap<T>::extract_min() {               

	HeapNode<T> temp = heap[0];
	heap[0] = heap[heap.size() - 1];
	heap.erase(heap.end()-1);		// this finds the minimum node of the tree

	MinHeapify(0);				// this tree arranges it in the form of min heap

	return temp.data;
}

// This helper function is used to create the min heap
template <typename T>
void MinHeap<T>::MinHeapify(const int idx)

{

	int smallest = idx;	
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < heap.size() && heap[left].key < heap[smallest].key)			// if the key of the smallest is more than the left
		smallest = left;

	if (left != right) {

		if (right < heap.size() && heap[right].key < heap[smallest].key)	// if the key of the smallest is more than the right
			smallest = right;
	}
	else
		smallest = left;

	if (smallest != idx) {								// if the smallest is less than the index of the node
		HeapNode<T> temp = heap[smallest];
		heap[smallest] = heap[idx];
		heap[idx] = temp;

		MinHeapify(smallest);							// recursively calling the function
	}
}

// this function to used to look at the data of the node  
template <typename T>
T MinHeap<T>::peek() const {
	int len = size();
	T data=0;
	if (len != 0)
		data=heap[0].data;						// outputs the data at the zero index of the tree 				
	else
		cout << "no tree was given" << endl;				// if the tree is empty
	return data;
}


