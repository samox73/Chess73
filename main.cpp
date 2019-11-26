
#include "chess.hpp"

/* First of, I am sorry, if I misunderstood the goals of this exercise
 * I hope that this is not much more, than was asked for
 * This program is a minimalistic chess engine, with only the basic mechanics
 * 1. round: figures can be places using the algebraic notation, until the key-
 *			 word "next" is given.
 * 2. round: white and black take turns one after another and can capture each
 *			 others pieces.
 *
 * The function getPossibleCaptures(bool is_white, string cell_position) returns
 *	all possible moves of the specified figure at >>cell_position<< to capture
 *	an enemy figure, which was the functionality that was asked for in the
 *	exercise description.
 */

using namespace std;

int main() {
	
	bool place_figures = true;
	bool take_turns = false;
	bool white_is_next = true;
	bool load_from_file = true;  // if false, place figures manually

	string algebraic_move;
	bool color;  // 1 is white, 0 is black

	printWelcomeScreen();
	
	ChessBoard chess_board;
	chess_board.init();

	// place figures manually
	if(!load_from_file) {
		// place white figures
		color = 1;
		cout << "Starting to place white pieces!" << endl;
		while(place_figures) {
			cout << "Enter an algebraic notation to place figure (e.g. 'Ka1', 'next' to place black pieces):" << endl;
			cin >> algebraic_move;
			if(algebraic_move == "next") {
				place_figures = false;
				break;
			}
			chess_board.position(algebraic_move, color);
			chess_board.print();
		}
		
		// place black figures
		color = 0;
		cout << "Starting to place black pieces!" << endl;
		place_figures = true;
		while(place_figures) {
			cout << "Enter an algebraic notation to place figure (e.g. 'Ka1', 'next' to finally play!" << endl;
			cin >> algebraic_move;
			if(algebraic_move == "next") {
				take_turns = true;
				place_figures = false;
				break;
			}
			chess_board.position(algebraic_move, color);
			chess_board.print();
		}
	} else {
		// Load board from file
		// chess_board.saveBoard("starting_config.txt");
		chess_board.loadBoard("starting_config.txt");
		chess_board.print();
		take_turns = true;
	}

	// Main loop, consecutively lets white and black make a move
	while(take_turns) {
		(white_is_next) ? cout << " > White, make move: " : cout << " > Black, make move: ";
		cin >> algebraic_move;
		if (chess_board.makeMove(algebraic_move, white_is_next))  // make move and check if move was valid
																  // if not, while loop enters with same
																  // value for white_is_next
			white_is_next = !white_is_next;
		chess_board.print();
	}

	return 1;
}
