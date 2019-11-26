#include <vector>
#include <algorithm>  // std::find
#include "printing_utils.hpp"

#ifndef VALID_FIGS
#define VALID_FIGS
vector<char> VALID_FIGURES {{'K', 'Q', 'R', 'B', 'N', 'p'}};  // valid figures on the board
#endif

string figureToLocation(string in) {
	/* Converts a string in 'Ka1' notation to only the location, e.g.
	 * Ka1 -> a1
	 * Qb3 -> b3
	 */
	string out = "";
	out += in[1];
	out += in[2];
	return out;
}

string fullNotationToTargetPosition(string notation_input) {
	/* Converts full move to only the target position, e.g.
	 * Bf1b5 -> b5
	 */
	string move_notation = "";
	move_notation += notation_input[3]; // b5
	move_notation += notation_input[4];
	return move_notation;
}

string fullNotationToOriginalFigure(string notation_input) {
	/* Converts full move to only the original figure, e.g.
	 * Bf1b5 -> Bf1
	 */
	string figure_notation = "";
	figure_notation += notation_input[0]; // Bf1
	figure_notation += notation_input[1];
	figure_notation += notation_input[2];
	return figure_notation;
}

string fullNotationToTargetFigure(string notation_input) {
	/* Converts full move to only the target figure, e.g.
	 * Bf1b5 -> Bb5
	 */
	string new_figure_notation = "";
	new_figure_notation += notation_input[0]; // Bf5
	new_figure_notation += notation_input[3];
	new_figure_notation += notation_input[4];
	return new_figure_notation;
}

bool indicesValid(int row, int col) {
	/* checks if values are inside [0,7]
	 */
	return (row >= 0 && row < 8 && col >= 0 && col < 8);
}

// convert e.g. rowColToAlgebraic(2,1) to "b3"
string rowColToAlgebraic(int row, int col) {
	/* Takes two ints as argument and returns algebraic position, e.g.
	 * (row, col) -> (col)(row)
	 * (2,3)	  -> d3
	 */
	string notation = "";
	notation += col + 'a';      // add column as a char
	notation += row + '0' + 1;  // add row as a number (but still a char)
	return notation;
}

// convert e.g. "b3" to vector (2,1)
vector<int> algebraicToVector(string notation) {
	/* Conerts an algebraic position to a vector with the coordinates, e.g.
	 * (col)(row) -> (row, col)
	 * d3         -> (2,3)
	 */
	vector<int> vector;
	vector.push_back(notation[1] - '0' -1);
	vector.push_back(notation[0] - 'a');
	return vector;
}

// check if algebraic notation is valid (length, figure character etc.)
// ---------------- DEPRECATED -----------------
//
// bool isValidAlgebraicNotation(string notation) {
// 	if (notation.length() != 3) {
// 		printError(notation + " is not a valid algebraic notation! Must have length 3!");
// 		return false;
// 	}
// 
// 	char figure = notation[0];
// 	if (find(VALID_FIGURES.begin(), VALID_FIGURES.end(), figure) == VALID_FIGURES.end()) {
// 		printError(notation + " is not a valid figure! Must be 'K', 'Q', 'R', 'B', 'N' or 'p'!");
// 		return false;
// 	}
// 
// 	int col = notation[1] - 'a';
// 	int row = notation[2] - '0' - 1;
// 	if(!indicesValid(col,row)) {
// 		printError(notation + "is not a valid algebraic notation! Index out of bounds!");
// 		return false;
// 	}
// 
// 	return true;  // if all checks passed -> valid
// }
