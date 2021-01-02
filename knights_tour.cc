/////
// Name: Prachetas Deshpande
// project 3
// CS 3610
/////
#include "knights_tour.h"
#include <iostream>
#include <iomanip>
using namespace std;

KnightsTour::KnightsTour(int board_size) {
	this->board_size = board_size;

	this->board.resize(board_size);
	for (int i = 0; i < board_size; ++i) {
		this->board[i].resize(board_size);
	}
}

void KnightsTour::print() {
	for (int i = 0; i < this->board_size; i++) {
		for (int j = 0; j < this->board_size; j++)
			cout << setw(4) << this->board[i][j] << " ";
		cout << endl;
	}
	cout << endl << endl;
}

// Function: get_moves()
//    Desc: Get the row and column indices of all valid
//          knight moves reachable from position row, col.
//          An invalid move would be one that sends the
//          knight off the edge of the board or
//          to a position that has already been visited.
//          
//    int row         - Current row position of knight.
//    int col         - Current column position of knight.
//    int row_moves[] - Array to store row indices
//                      of all valid new moves reachable from
//                      the current position row, col.
//    int col_moves[] - Array to store column indices
//                      of all valid new moves reachable from
//                      the current position row, col.
//    int num_moves -   Number of valid moves found. Corresponds
//                      to the sizes of row_moves and col_moves.

void KnightsTour::get_moves(int row, int col,int row_moves[], int col_moves[], int& num_moves) {
	//num_moves = 0;
	int row_ref[] = { -1,1,2,2,1,-1,-2,-2 };
	int col_ref[] = { 2,2,1,-1,-2,-2,-1,1 };
	for (int i = 0; i <= 7; i++) {
		if (is_valid(row + row_ref[i], col + col_ref[i]) == true) {
			if (board[row + row_ref[i]][col + col_ref[i]] == 0) {
				row_moves[num_moves] = row + row_ref[i];
				col_moves[num_moves] = col + col_ref[i];
				num_moves++;
			}
		}
	}
}

bool KnightsTour::is_valid(int row, int col) {
	if (row <= 4 && row >= 0 && col <= 4 && col >= 0)
		return true;
	else
		return false;
}

// Function: move() --> Recursive
//     int row        - Current row position of knight.
//     int col        - Current column position of knight.
//     int& m         - Current move id in tour.
//                      Stored in board at position
//                      row, col.
//     int& num_tours - Total number of tours found.

void KnightsTour::move(int row, int col, int& m, int& num_tours) {
	m++;
	board[row][col] = m;
	int new_row[8];
	int new_col[8];
	if (m == board_size*board_size) {
		print();
		m--;
		board[row][col] = 0;
		num_tours++;
		return;
	}
	int num_moves = 0;
	get_moves(row, col, new_row, new_col, num_moves);	
	int i = 0;
	while (i < num_moves && num_moves!=0) {
		move(new_row[i], new_col[i], m, num_tours);
		i++;
	}
	board[row][col] = 0;
	m--;
}

int KnightsTour::generate(int row, int col) {
	int m = 0;
	int num_tours = 0;
	move(row, col, m, num_tours);

	return num_tours;
}

