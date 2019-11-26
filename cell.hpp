#include "chess_utils.hpp"

// struct, that represents one cell on a chess board
struct Cell {
	char _figure;
	bool _empty;
	bool _white;
	string _location;

	Cell() {
		_figure = ' ';
		_empty = true;
		_white = false;
		_location = "";
	}

	Cell(int row, int col) {
		_figure = ' ';
		_empty = true;
		_white = false;
		this->setLocation(row, col);
	}

	void placeFigure(char figure, bool white) {
		_figure = figure;
		_white = white;
		setEmpty(false);
	}

	void setLocation(string notation) {  // sets notation through algebraic notation, e.g. Ka1 sets notation = a1
		_location = notation;
	}

	void setLocation(int row, int col) {
		_location = rowColToAlgebraic(row, col);
	}

	string getNotation() {
		return _figure + _location;
	}

	char getFigure() {
		return _figure;
	}

	bool isEmpty() {
		return _empty;
	}

	bool isWhite() {
		return _white;
	}

	void setEmpty(bool empty) {
		_empty = empty;
	}

	void removeFigure() {
		_empty = true;
		_figure = ' ';
	}
};
