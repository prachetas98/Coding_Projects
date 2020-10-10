
#ifndef OTHELLO_GAME
#define OTHELLO_GAME
#include "game.h"
#include "piece.h"
#include <string>
#define MAX 8
using namespace std;
namespace main_savitch_14
{
	class othello : public game
	{
	public:

		othello();
		~othello();
		void initialization();
	protected:

		void make_move(const std::string& move);
		// Restart the game from the beginning:
		void restart();

		// *******************************************************************
		// PURE VIRTUAL FUNCTIONS
		// *******************************************************************
		// (these must be provided for each derived class)
		// Return a pointer to a copy of myself:
		othello* clone() const { return new othello(*this); }
		// Compute all the moves that the next player can make:
		void compute_moves(std::queue<std::string>& moves) const;
		std::string get_user_move() const;
		// Display the status of the current game:
		void display_status() const;
		// Evaluate a board position:
		// NOTE: positive values are good for the computer.
		int evaluate() const; //{ return 1; }
		// Return true if the current game is finished:
		bool is_game_over() const;
		// Return true if the given move is legal for the next player:
		who winning() const;
		bool is_legal(const std::string& move) const;
		bool legality(int column, int row, int color_of_player) const;
		bool is_below(int column, int row, int color_of_player) const;
		bool is_above(int column, int row, int color_of_player) const;
		bool is_left(int column, int row, int color_of_player) const;
		bool is_right(int column, int row, int color_of_player) const;
		bool is_below_left(int column, int row, int color_of_player) const;
		bool is_above_left(int column, int row, int color_of_player) const;
		bool is_below_right(int column, int row, int color_of_player) const;
		bool is_above_right(int column, int row, int color_of_player) const;

	private:
		piece p[MAX][MAX];
		bool white_passed;
		bool black_passed;
	};
}
#endif

