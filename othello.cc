
#include "othello.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <queue>
using namespace std;

namespace main_savitch_14 {

	othello::othello() : game()
	{
		initialization();
	}


	othello::~othello()
	{
	}

	void othello::display_status()const {
		char name = 'A';
		cout << B_RED << "  ";
		for (int i = 0; i < MAX; ++i) {
			cout << B_RED << "-- " << name << " -- " << RESET;
			name++;
		}
		cout << GREEN << "\n |";
		for (int i = 0; i < MAX; ++i) {
			cout << "       |";
		}

		for (int i = 0; i < MAX; ++i) {
			cout << endl << YELLOW << (i + 1) << GREEN << "|";
			for (int j = 0; j < 8; ++j) {
				p[j][i].output();
				cout << GREEN << "|";
			}
			cout << GREEN << "\n |";
			for (int i = 0; i < MAX; ++i) {
				cout << "_______|";
			}
		}
		cout << RESET << endl << endl;


	}

	void othello::initialization() {
		white_passed = false;
		black_passed = false;
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				p[i][j].set_color(0);

			}
		}
		p[3][3].set_color(1);
		p[3][4].set_color(2);
		p[4][3].set_color(2);
		p[4][4].set_color(1);
	}

	void othello::restart() {
		initialization();
		game::restart();
	}


	bool othello::is_legal(const std::string& move) const {
		bool legal_move = true;
		int color_of_player;
		if (next_mover() == HUMAN) {
			color_of_player = 2;
		}
		else if (next_mover() == COMPUTER) {
			color_of_player = 1;
		}

		if (move == "PASS") {
			legal_move = true;

		}
		else {
			char a = move.at(0);
			a = toupper(a);
			int column = a - 65;
			char b = move.at(1);
			int row = b - 49;

			legal_move = legality(column, row, color_of_player);
		}
		return legal_move;
	}

	bool othello::legality(int column, int row, int color_of_player) const {
		bool legal = false;
		if (p[column][row].get_color() != 0) {
			return legal;
		}
		if (legal == false)
			legal = is_below(column, row, color_of_player);
		if (legal == false)
			legal = is_above(column, row, color_of_player);
		if (legal == false)
			legal = is_left(column, row, color_of_player);
		if (legal == false)
			legal = is_right(column, row, color_of_player);
		if (legal == false)
			legal = is_below_left(column, row, color_of_player);
		if (legal == false)
			legal = is_below_right(column, row, color_of_player);
		if (legal == false)
			legal = is_above_left(column, row, color_of_player);
		if (legal == false)
			legal = is_above_right(column, row, color_of_player);

		return legal;
	}
	bool othello::is_below(int column, int row, int color_of_player) const {
		bool below = false;
		if (row == MAX - 1) {
			return below;
		}
		if (p[column][row + 1].get_color() == 0 || p[column][row + 1].get_color() == color_of_player) {
			return below;
		}
		for (int i = row + 2; i < MAX; i++) {
			if (p[column][i].get_color() == 0)
				return below;
			if (color_of_player == p[column][i].get_color()) {
				below = true;
			}
		}
		return below;
	}

	bool othello::is_above(int column, int row, int color_of_player) const {
		bool above = false;
		if (row == 0) {
			return above;
		}
		if (p[column][row - 1].get_color() == 0 || p[column][row - 1].get_color() == color_of_player) {
			return above;
		}
		for (int i = row - 2; i >= 0; i--) {
			if (p[column][i].get_color() == 0)
				return above;
			if (color_of_player == p[column][i].get_color()) {
				above = true;
			}
		}
		return above;
	}

	bool othello::is_left(int column, int row, int color_of_player) const {
		bool left = false;
		if (column == 0) {
			return left;
		}
		if (p[column - 1][row].get_color() == 0 || p[column - 1][row].get_color() == color_of_player) {
			return left;
		}
		for (int i = column - 2; i >= 0; i--) {
			if (p[i][row].get_color() == 0)
				return left;
			if (color_of_player == p[i][row].get_color()) {
				left = true;
			}
		}
		return left;
	}

	bool othello::is_right(int column, int row, int color_of_player) const {
		bool right = false;
		if (column == MAX - 1) {
			return right;
		}
		if (p[column + 1][row].get_color() == 0 || p[column + 1][row].get_color() == color_of_player) {
			return right;
		}
		for (int i = column + 2; i < MAX; i++) {
			if (p[i][row].get_color() == 0)
				return right;
			if (color_of_player == p[i][row].get_color()) {
				right = true;
			}
		}
		return right;
	}


	bool othello::is_below_left(int column, int row, int color_of_player) const {
		bool below_left = false;
		if (column == 0 || row == MAX - 1) {
			return below_left;
		}
		int i = column - 1;
		int j = row + 1;
		if (p[i][j].get_color() == 0 || p[i][j].get_color() == color_of_player) {
			return below_left;
		}
		i = column - 2;
		j = row + 2;
		while (i >= 0 && j < MAX) {
			if (p[i][j].get_color() == 0)
				return below_left;
			if (color_of_player == p[i][j].get_color()) {
				below_left = true;
				break;
			}
			i--;
			j++;
		}
		return below_left;
	}

	bool othello::is_above_left(int column, int row, int color_of_player) const {
		bool above_left = false;
		if (column == 0 || row == 0) {
			return above_left;
		}
		if (p[column - 1][row - 1].get_color() == 0 || p[column - 1][row - 1].get_color() == color_of_player) {
			return above_left;
		}
		int i = column - 2;
		int j = row - 2;
		while (i >= 0 && j >= 0) {
			if (p[i][j].get_color() == 0)
				return above_left;
			if (color_of_player == p[i][j].get_color()) {
				above_left = true;
				break;
			}
			i--;
			j--;
		}
		return above_left;
	}

	bool othello::is_below_right(int column, int row, int color_of_player) const {
		bool below_right = false;
		if (column == MAX - 1 || row == MAX - 1) {
			return below_right;
		}
		if (p[column + 1][row + 1].get_color() == 0 || p[column + 1][row + 1].get_color() == color_of_player) {
			return below_right;
		}
		int i = column + 2;
		int j = row + 2;
		while (i < MAX && j < MAX) {
			if (p[i][j].get_color() == 0)
				return below_right;
			if (color_of_player == p[i][j].get_color()) {
				below_right = true;
				break;
			}
			i++;
			j++;
		}
		return below_right;
	}

	bool othello::is_above_right(int column, int row, int color_of_player) const {
		bool above_right = false;
		if (column == MAX - 1 || row == 0) {
			return above_right;
		}
		if (p[column + 1][row - 1].get_color() == 0 || p[column + 1][row - 1].get_color() == color_of_player) {
			return above_right;
		}
		int i = column + 2;
		int j = row - 2;
		while (i < MAX && j >= 0) {
			if (p[i][j].get_color() == 0)
				return above_right;
			if (color_of_player == p[i][j].get_color()) {
				above_right = true;
				break;
			}
			i++;
			j--;
		}
		return above_right;
	}

	void othello::make_move(const std::string& move) {
		int color_of_player;
		if (next_mover() == HUMAN) {
			color_of_player = 2;
		}
		else if (next_mover() == COMPUTER) {
			color_of_player = 1;
		}
		if (move != "PASS") {
			char a = move.at(0);
			a = toupper(a);
			int column = a - 65;
			char b = move.at(1);
			int row = b - 49;
			//int color_of_player;
			p[column][row].set_color(color_of_player);
			if (is_below(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i][j + 1].get_color() != color_of_player && j < MAX) {
					p[i][j + 1].flip();
					j++;
				}
			}
			if (is_above(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i][j - 1].get_color() != color_of_player && j >= 0) {
					p[i][j - 1].flip();
					j--;
				}
			}
			if (is_left(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i - 1][j].get_color() != color_of_player && i >= 0) {
					p[i - 1][j].flip();
					i--;
				}
			}
			if (is_right(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i + 1][j].get_color() != color_of_player && i < MAX) {
					p[i + 1][j].flip();
					i++;
				}
			}
			if (is_below_left(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i - 1][j + 1].get_color() != color_of_player && j < MAX && i >= 0) {
					p[i - 1][j + 1].flip();
					i--;
					j++;
				}
			}
			if (is_above_left(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i - 1][j - 1].get_color() != color_of_player && i >= 0 && j >= 0) {
					p[i - 1][j - 1].flip();
					i--;
					j--;
				}
			}
			if (is_below_right(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i + 1][j + 1].get_color() != color_of_player && j < MAX && i < MAX) {
					p[i + 1][j + 1].flip();
					i++;
					j++;
				}
			}
			if (is_above_right(column, row, color_of_player) == true) {
				int i = column;
				int j = row;
				while (p[i + 1][j - 1].get_color() != color_of_player && j >= 0 && i < MAX) {
					p[i + 1][j - 1].flip();
					i++;
					j--;
				}

			}

			if (color_of_player == 1) {
				white_passed = false;
			}
			if (color_of_player == 2) {
				black_passed = false;
			}
		}

		else {
			if (color_of_player == 1) {
				white_passed = true;
			}
			else if (color_of_player == 2) {
				black_passed = true;
			}
		}
		game::make_move(move);
	}
	game::who othello::winning() const {
		int black_moves = 0;
		int white_moves = 0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (p[i][j].get_color() == 1) {
					white_moves++;
				}
				else if (p[i][j].get_color() == 2) {
					black_moves++;
				}
			}
		}
		if (black_moves > white_moves) {
			cout << "BLACK WINS" << endl;
			return game::HUMAN;
		}
		else if (black_moves < white_moves) {
			cout << "WHITE WINS" << endl;
			return game::COMPUTER;
		}
		else {
			cout << "THE SCORE IS TIED" << endl;
			return game::NEUTRAL;
		}

	}

	void othello::compute_moves(std::queue<std::string>& moves) const {
		string my_move("  ");
		my_move[0] = 'A';
		my_move[1] = '1';
		for (char column_number = 'A'; column_number < 'I'; column_number++) {
			for (char row_number = '1'; row_number < '9'; row_number++) {
				my_move[0] = column_number;
				my_move[1] = row_number;
				if (is_legal(my_move))
					moves.push(my_move);
			}
		}
	}

	std::string othello::get_user_move() const {
		std::queue<std::string> moves_made;
		compute_moves(moves_made);
		if (moves_made.empty() == true) {
			return "PASS";
		}
		else {
			return game::get_user_move();

		}
	}

	bool othello::is_game_over() const {
		//std::queue<std::string> number_of_moves;
		
		if (white_passed == true && black_passed == true) {
			return true;
		}
		/*
		compute_moves(number_of_moves);
		if (number_of_moves.empty() ==true ) {
			return true;
		}
		*/
		else {
			return false;
		}
		
	}

	int othello::evaluate()const {
		int black_moves = 0;
		int white_moves = 0;
		int simplify=0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (p[i][j].get_color() == 1) {
					black_moves++;
				}
				else if (p[i][j].get_color() == 2) {
					white_moves++;
				}
			}
		}
		if (black_moves > white_moves)
			simplify = black_moves - 64;
		else if (white_moves > black_moves)
			simplify = 64-white_moves;
		return simplify;
	}

}
