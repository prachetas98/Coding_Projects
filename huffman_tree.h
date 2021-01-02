/////
// Name: Prachetas Deshpande
// CS 3610
// Project 2
/////
#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include <map>
#include <vector>
#include <iterator>
using namespace std;

struct HuffmanNode {
	HuffmanNode(char character, int frequency) :
		character(character), frequency(frequency),
		left(NULL), right(NULL) {}				// constructor to initialize the private variables

	HuffmanNode(int frequency) :					// constructor to obtain the astriks of the huffman node
		character('*'), frequency(frequency),
		left(NULL), right(NULL) {}

	HuffmanNode(const HuffmanNode& h1) { character = h1.character; 
	frequency = h1.frequency;					// the default constructor
	left = h1.left;
	right = h1.right;
	}
	
	~HuffmanNode() {
		delete left;						// the default destructor
		delete right;
		left = right = NULL;
	}

	char character;
	int frequency;
	HuffmanNode *left, *right;
};

class HuffmanTree {
public:
	HuffmanTree() : root(NULL), message("") {}			// default constructor of huffman tree
	~HuffmanTree() { delete this->root; }				// default destructor of huffman tree

	void construct(const string message);
	void destruct() { delete this->root; this->root = NULL; message = ""; }
	void print() const;
	void printCodes(HuffmanNode* node, vector<char> str, int top, map<char,string>& code_map) const;
	int height(HuffmanNode* root) const;
private:

	HuffmanNode *root;
	string message;
};

#endif
