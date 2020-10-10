
#ifndef OTHELLO_PIECE
#define OTHELLO_PIECE
#include <iostream>
#include "colors.h"
using namespace std;
namespace main_savitch_14 {
	class piece
	{
	public:
		piece() {}
		int get_color() const { return colors; }
		void set_color(int i) { colors = i; }
		void output()const {
			if (colors == 0) {
				cout << GREEN << "EMPTY  ";
			}
			if (colors == 1) {
				cout << WHITE << " WHITE ";
			}
			if (colors == 2) {
				cout << BLUE << " BLACK ";
			}
		}
		void flip() {
			if (colors == 1) {
				colors = 2;

			}
			else if (colors == 2) {
				colors = 1;
			}
		}
	private:
		int colors;

	};
}

#endif


