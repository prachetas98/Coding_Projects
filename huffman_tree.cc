/////
// Name: Prachetas Deshpande
// Project 2
// CS 3610
/////
#include "huffman_tree.h"
#include <iostream>
#include "min_heap.h"
#include <string>
#include <map>
using namespace std;

void HuffmanTree::construct(const string message) {				// function to construct the huffman tree
	this->message = message;

	// Count the frequency of each letter in message
	// e.g. 
	//    message == "aaabbccccdd"
	//    frequencies == {a:3, b:2, c:4, d:2} 
	map<char, int> frequency_map;
	for (int i = 0; i < message.length(); ++i) {
		if (frequency_map.find(message[i]) != frequency_map.end())
			++frequency_map[message[i]];
		else
			frequency_map[message[i]] = 1;
	}

	// Create HuffmanNode for each unique letter in message
	// and organize nodes into a min heap
	// e.g.
	//  heap == 
	//            {b:2}  
	//           /     \
	  //        {d:2}   {a:3}
//        /   \   /    \
  //      {c:4} 
	MinHeap<HuffmanNode*> heap;
	map<char, int>::iterator it = frequency_map.begin();
	for (; it != frequency_map.end(); ++it) {
		HuffmanNode* node = new HuffmanNode(
			it->first, it->second
		);
		heap.insert(node, it->second);
	}
	
	// Combine nodes with smallest frequency and insert
	// back into heap until heap size == 1. Along the way,
	// create binary tree out of combined nodes.
	// e.g.
	//  (1)
	//     {b:2} == heap.extract_min()
	//     {d:2} == heap.extract_min()
	//     parent ==
	//               {*:4}
	//              /     \
	  //            {b:2}  {d:2}
//
//     heap == 
//              {a:3}
//             /     \
  //           {c:4}   {*:4}
//
//  (2)
//     {a:3} == heap.extract_min()
//     {c:4} == heap.extract_min()
//     parent ==
//              {*:7}
//             /     \
  //          {a:3}   {*:4}
//    
//     heap ==
//            {c:4}
//           /
//         {*:7}
//
//  (3)
//     {*:4} == heap.extract_min()
//     {*:7} == heap.extract_min()
//     parent ==
//            {*:11}
//           /       \
  //      {c:4}        {*:7}
//                  /     \
  //                {a:3}  {*:4}
//                       /    \
  //                    {b:2}  {d:2}
//
//     heap == {*:11}
	
	while (heap.size() > 1) {
		HuffmanNode *left, *right;

		left = heap.extract_min();
		right = heap.extract_min();
	
	
		
		HuffmanNode *parent = new HuffmanNode(
			left->frequency + right->frequency
		);
		parent->left = left;
		parent->right = right;

		heap.insert(parent, parent->frequency);
	}
	
	// Get root of huffman tree. e.g. {*:11}
	this->root = heap.peek();
	
}

// this function is used to print the code out of the map
void HuffmanTree::print() const {						 
	int height_of_tree = height(root);
	vector<char> str(height_of_tree);
	int top = 0;
	map<char, string> code_map;						// map to store the codes
	printCodes(root, str, top, code_map);					// function that prints the code
	map<char, string>::iterator itr;
	int len = message.length();
	string code = "";							// string that conatins the code of the message
	for (int i = 0; i < len; i++) {
		for (itr = code_map.begin(); itr != code_map.end(); ++itr) {
			if (message[i] == itr->first)
				code = code + itr->second + " ";
		
		}
	}
	std::cout << code << " ";
	code_map.clear();
}

// Function to obtain the encoded message from our tree.
void HuffmanTree::printCodes(HuffmanNode* node, vector<char> str, int top, map<char,string>& code_map) const {	
		// Assign 0 to left edge and recur 
		if (node->left) {
			str[top] = '0';
			printCodes(node->left, str, top + 1,code_map);
	}

	// Assign 1 to right edge and recur 
	if (node->right) {

		str[top] = '1';
		printCodes(node->right, str, top + 1, code_map);
	}
	if (node->left==NULL && node->right==NULL) {
		string temp="";
		for (int i = 0; i < top; i++) {
			temp = temp + str[i];
		}
		code_map.insert(pair<char, string>(node->character, temp));
	}

}

// Function to obtain the height of the tree
int HuffmanTree::height(HuffmanNode* root) const
{
	// Base case: empty tree has height 0
	if (root == nullptr)
		return 0;

	return max(height(root->left), height(root->right)) + 1;
}




