#include "cell.hpp"
#include <fstream>

struct ChessBoard {
	vector<vector<Cell>> _board;
	int _rows = 8, _columns = 8;
	
	void init();
	void position(string algebraic_move, bool white);
	void print(int offset);
	vector<string> getPossibleCaptures(bool white, string figure_notation);
	vector<string> getPossibleMoves(string figure_notation); // cell_location in alg.notation, e.g. "e5"
	vector<string> getKingMoves(int row, int col, bool is_white);
	vector<string> getQueenMoves(int row, int col, bool is_white);
	vector<string> getRookMoves(int row, int col, bool is_white);
	vector<string> getBishopMoves(int row, int col, bool is_white);
	vector<string> getKnightMoves(int row, int col, bool is_white);
	vector<string> getPawnMoves(int row, int col, bool is_white);
	bool isCaptureable(string cell_position, bool attacker_is_white);
	vector<string> getWhiteFigures();
	vector<string> getBlackFigures();
	bool makeMove(string notation_input, bool white_is_next);
	Cell& getCell(string location_notation);
	void saveBoard(string filename);
	void loadBoard(string filename);
	bool kingIsCheck(bool is_white);
	bool isValidMove(string notation_input, bool is_white);
	bool simulateMove(string notation_input, bool is_white, bool verbose);
	bool isCheckmate(bool is_white);
};

// init an empty board, fill with empty cells
void ChessBoard::init() {
	_board = vector<vector<Cell>>();
	for (size_t row = 0; row < _rows; row++) {
		_board.push_back(vector<Cell>());
		for (size_t col = 0; col < _columns; col++) {
			_board[row].push_back(Cell(row, col));
		}
	}
}


// place a piece of the specified color on the board in algebraic notation
void ChessBoard::position(string notation, bool is_white) {

	// Check if input has length 3, if not throw error. valid format is e.g. "Ke3"
	if(notation.length() != 3) 
		throw runtime_error("Notation string has length " + to_string(notation.length()) + " but expected is a value of 3!");

	char figure = notation[0];        // first char is the figure char
	int col = notation[1] - 'a';	  // the col-char minus 'a' gives the column as an int
	int row = notation[2] - '0' - 1;  // the row as an int
	
	if (_board[row][col].isEmpty() || _board[row][col].isWhite() ^ is_white) {  // if empty or different color
		_board[row][col].placeFigure(figure, is_white);  // place/replace figure on board
	} else {
		// if not empty or same color, throw error
		throw runtime_error("A figure is already at " + to_string(row) + "|" + to_string(col) + " (row|col)!");
	}
}

void ChessBoard::print(int offset = 13) {
	/* Method to print the current board
	 * (self-explanatory)
	 */
	cout << getNChars(' ', offset);
	cout << "       a      b      c      d      e      f      g      h" << endl;
	cout << getNChars(' ', offset);
	cout << "    --------------------------------------------------------   " << endl;

	for (size_t row = 7; row < _rows; row--) {  // print rows from 8 to 1 (top to bottom, 
												// as usual in chess)
		cout << getNChars(' ', offset);
		cout << " " << row+1 << " |";
		for (size_t col = 0; col < _columns; col++) {
			cout << " " << _board[row][col].getFigure();
			if (!_board[row][col].isEmpty()) {
				(_board[row][col].isWhite()) ? cout << "(w)" : cout << "(k)";
			} else {
				cout << "   ";
			}
			cout << " |";
		}
		cout << " " << row+1 << endl;
		cout << getNChars(' ', offset);
		cout << "    --------------------------------------------------------   " << endl;
	}

	cout << getNChars(' ', offset);
	cout << "       a      b      c      d      e      f      g      h" << endl;
}

// determines if the cell can be captured by the attacker_color
bool ChessBoard::isCaptureable(string cell_position, bool attacker_is_white) {
	vector<int> position {algebraicToVector(cell_position)};  // convert e.g. b2 to (1,1)
	int row {position[0]}, col {position[1]};

	// if NOT empty and DIFFERENT color (^ is the XOR operator) -> capturable, false otherwise
	if (!_board[row][col].isEmpty() && attacker_is_white ^ _board[row][col].isWhite()) {
		return true;
	} else {return false;}
}

vector<string> ChessBoard::getPossibleCaptures(bool is_white, string figure_notation) {
	/* returns a vector of strings with all possible captures of a piece
	 */
	vector<string> possible_moves {getPossibleMoves(figure_notation)};
	vector<string> possible_captures;
	string figure;

	for (auto cell_pos : possible_moves) {
		if (isCaptureable(cell_pos, is_white))
			figure = getCell(cell_pos).getNotation();
			possible_captures.push_back(figure);
	}
	return possible_captures;
}

vector<string> ChessBoard::getPossibleMoves(string figure_notation) {
	/* returns a string vector of all possible moves of a piece
	 */
	vector<string> possible_moves;
	int col = figure_notation[1] - 'a';
	int row = figure_notation[2] - '0' - 1;
	bool is_white = _board[row][col].isWhite();

	switch (_board[row][col].getFigure()) {
		case 'K':
			return this->getKingMoves(row, col, is_white);
			break;
		case 'Q':
			return this->getQueenMoves(row, col, is_white);
			break;
		case 'R':
			return this->getRookMoves(row, col, is_white);
			break;
		case 'B':
			return this->getBishopMoves(row, col, is_white);
			break;
		case 'N':
			return this->getKnightMoves(row, col, is_white);
			break;
		case 'p':
			return this->getPawnMoves(row, col, is_white);
			break;

		default:
			throw invalid_argument("Cannot find possible moves for empty cells!");
	}
}

/* =====================================================================================
   ========= THE NEXT 6 METHODS DESCRIBE THE MECHANICS HOW FIGURES CAN MOVE ============
   ===================================================================================== */

vector<string> ChessBoard::getKingMoves(int row, int col, bool is_white) {
	vector<string> moves;
	for (int pos_row = row-1; pos_row < row+2; pos_row++) {
		for (int pos_col = col-1; pos_col < col+2; pos_col++) {
			if (indicesValid(pos_row, pos_col)) {
				if (pos_row != row || pos_col != col) {
					if (_board[pos_row][pos_col].isWhite() ^ is_white || _board[pos_row][pos_col].isEmpty())  // XOR -> only possible if color is different, or if field is empty
					moves.push_back(rowColToAlgebraic(pos_row, pos_col));
				}
			}
		}
	}
	return moves;
}

vector<string> ChessBoard::getQueenMoves(int row, int col, bool is_white) {
	vector<string> moves = getRookMoves(row, col, is_white);
	vector<string> bishopMoves = getBishopMoves(row, col, is_white);
	moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
	return moves;
}

vector<string> ChessBoard::getRookMoves(int row, int col, bool is_white) {
	vector<string> moves;
	// "shoot" into all directions and detect collisions
	for (size_t pos_row = row+1; pos_row < 8; pos_row++) {
		if (_board[pos_row][col].isWhite() ^ is_white || _board[pos_row][col].isEmpty()) { // XOR -> only
													// possible if color is different, or if field is empty
			moves.push_back(rowColToAlgebraic(pos_row, col));
			if (!_board[pos_row][col].isEmpty()) break;  // break out if enemy was encountered
		} else {
			break;
		}
	}
	for (int pos_row = row-1; pos_row >= 0; pos_row--) {
		if (_board[pos_row][col].isWhite() ^ is_white || _board[pos_row][col].isEmpty()) {
			moves.push_back(rowColToAlgebraic(pos_row, col));
			if (!_board[pos_row][col].isEmpty()) break;  // break out if enemy was encountered
		} else {
			break;
		}
	}
	for (size_t pos_col = col+1; pos_col < 8; pos_col++) {
		if (_board[row][pos_col].isWhite() ^ is_white || _board[row][pos_col].isEmpty()) {
			moves.push_back(rowColToAlgebraic(row, pos_col));
			if (!_board[row][pos_col].isEmpty()) break;  // break out if enemy was encountered
		} else {
			break;
		}
	}
	for (int pos_col = col-1; pos_col >= 0; pos_col--) {
		if (_board[row][pos_col].isWhite() ^ is_white || _board[row][pos_col].isEmpty()) {
			moves.push_back(rowColToAlgebraic(row, pos_col));
			if (!_board[row][pos_col].isEmpty()) break;  // break out if enemy was encountered
		} else {
			break;
		}
	}
	return moves;
}

vector<string> ChessBoard::getBishopMoves(int row, int col, bool is_white) {
	vector<string> moves;
	// "shoot" into all diagonals and detect collisions 4 for loops to iterate over each direction
	for (size_t i = 1; i < 8; i++) {
		if (indicesValid(row+i, col+i)) {
			if (_board[row+i][col+i].isWhite() ^ is_white || _board[row+i][col+i].isEmpty()) {
				moves.push_back(rowColToAlgebraic(row+i, col+i));
				if (!_board[row+i][col+i].isEmpty()) break;  // break out if enemy was encountered
			} else { break; }
		} else { break; }
	}
	for (size_t i = 1; i < 8; i++) {
		if (indicesValid(row-i, col-i)) {
			if (_board[row-i][col-i].isWhite() ^ is_white || _board[row-i][col-i].isEmpty()) {
				moves.push_back(rowColToAlgebraic(row-i, col-i));
				if (!_board[row-i][col-i].isEmpty()) break;  // break out if enemy was encountered
			} else { break; }
		} else { break; }
	}
	for (size_t i = 1; i < 8; i++) {
		if (indicesValid(row-i, col+i)) {
			if (_board[row-i][col+i].isWhite() ^ is_white || _board[row-i][col+i].isEmpty()) {
				moves.push_back(rowColToAlgebraic(row-i, col+i));
				if (!_board[row-i][col+i].isEmpty()) break;  // break out if enemy was encountered
			} else { break; }
		} else { break; }
	}
	for (size_t i = 1; i < 8; i++) {
		if (indicesValid(row+i, col-i)) {
			if (_board[row+i][col-i].isWhite() ^ is_white || _board[row+i][col-i].isEmpty()) {
				moves.push_back(rowColToAlgebraic(row+i, col-i));
				if (!_board[row+i][col-i].isEmpty()) break;  // break out if enemy was encountered
			} else { break; }
		} else { break; }
	}
	return moves;
}

vector<string> ChessBoard::getKnightMoves(int row, int col, bool is_white) {
	vector<string> moves;
	for (int i {-2}; i < 3; i += 4) {  // jump 2 back/forth
		for (int j {-1}; j < 2; j += 2) {  // jump 1 left/right
			if (indicesValid(row+i, col+j)) {
				if (_board[row+i][col+j].isWhite() ^ is_white || _board[row+i][col+j].isEmpty())
					moves.push_back(rowColToAlgebraic(row+i, col+j));
			}
		}
	}
	for (int i {-2}; i < 3; i += 4) {  // jump 2 left/right
		for (int j {-1}; j < 2; j += 2) {  // jump 1 back/forth
			if (indicesValid(row+j, col+i)) {
				if (_board[row+j][col+i].isWhite() ^ is_white || _board[row+j][col+i].isEmpty())
					moves.push_back(rowColToAlgebraic(row+j, col+i));
			}
		}
	}
	return moves;
}

vector<string> ChessBoard::getPawnMoves(int row, int col, bool is_white) {
	vector<string> moves;
	int delta_row;
	(is_white) ? delta_row = 1 : delta_row = -1;
	if (indicesValid(row+delta_row, col)) {
		if (_board[row+delta_row][col].isEmpty())
			moves.push_back(rowColToAlgebraic(row+delta_row, col));
	}
	if (indicesValid(row+delta_row, col+1)) {
		if (_board[row+delta_row][col+1].isWhite() ^ is_white && !_board[row+delta_row][col+1].isEmpty())
			moves.push_back(rowColToAlgebraic(row+delta_row, col+1));
	}

	if (indicesValid(row+delta_row, col-1)) {
		if (_board[row+delta_row][col-1].isWhite() ^ is_white && !_board[row+delta_row][col-1].isEmpty())
			moves.push_back(rowColToAlgebraic(row+delta_row, col-1));
	}
	return moves;
}

/* =====================================================================================
   ======= THE PREVIOUS 6 METHODS DESCRIBE THE MECHANICS HOW FIGURES CAN MOVE ==========
   ===================================================================================== */

vector<string> ChessBoard::getWhiteFigures() {
	/* get all white pieces in the form of algebraic notation
	 *  e.g. {"Bc1", "Qd1", ... }
	 */
	vector<string> figures;
	for (auto row : _board) {
		for (auto cell : row) {
			if (cell.isWhite() && !cell.isEmpty()) {
				figures.push_back(cell.getNotation());
			}
		}
	}
	return figures;
}

vector<string> ChessBoard::getBlackFigures() {
	/* get all black pieces in the form of algebraic notation
	 *  e.g. {"Bc1", "Qd1", ... }
	 */
	vector<string> figures;
	for (auto row : _board) {
		for (auto cell : row) {
			if (!cell.isWhite() && !cell.isEmpty()) {
				figures.push_back(cell.getNotation());
			}
		}
	}
	return figures;
}

Cell& ChessBoard::getCell(string location_notation) {
	/* get the reference to the cell with the specified location
	 */
	Cell cell;
	vector<int> position = algebraicToVector(location_notation);
	return _board[position[0]][position[1]];
}

bool ChessBoard::makeMove(string notation_input, bool is_white) {
	/* checks if move if valid and if no rules are broken, and then
	 * performs the move. Also checks for check and checkmate, respectively.
	 */
	bool move_valid = false;

	vector<string> figures;
	(is_white) ? figures = getWhiteFigures() : figures = getBlackFigures();

	move_valid = isValidMove(notation_input, is_white);
	
	if (move_valid) {
		// Reference to current cell of figure
		Cell& figure = this->getCell(figureToLocation(fullNotationToOriginalFigure(notation_input)));
		figure.removeFigure();  // remove figure from current position
		this->position(fullNotationToTargetFigure(notation_input), is_white);  // place figure in new position
		
		//kingIsCheck(is_white);
		if(kingIsCheck(!is_white)) {
			printInfoBox("King is check!");
			if (isCheckmate(!is_white)) {
				printInfoBox("CHECKMATE, LOOOOSER!");
				exit(0);
			}
		}
		return true;
	}
	return false;
}

void ChessBoard::saveBoard(string filename) {
	/* saves board to file in a simply editable format 
	 */
	ofstream output_file(filename);
	if (output_file.is_open()) {
		for (size_t row = 7; row < _rows; row--) {  // print rows from 8 to 1 (top to bottom, 
													// as usual in chess)
			for (size_t col = 0; col < _columns; col++) {
				if (!_board[row][col].isEmpty()) {
					output_file <<  _board[row][col].getFigure();
					(_board[row][col].isWhite()) ? output_file << "(w)" : output_file << "(k)";
				} else {
					output_file << "----";
				}
			}
			output_file << endl;
		}
	}
	output_file.close();
}

void ChessBoard::loadBoard(string filename) {
	/* loads board from file in a simply editable format 
	 */
	ifstream input_file(filename);
	string line, cell;
	this->init();
	int row = 7, column = 0;

	if (input_file.is_open()) {
		while(getline(input_file, line)) {
			if (line.length() != 32)
				throw runtime_error("Invalid size of row when loading from file!");
			
			column = 0;
			for (size_t i {0}; i < 8; i++) {  // read next cell notation
				cell = "";
				cell += line[4*i];
				cell += line[4*i + 1];
				cell += line[4*i + 2];
				cell += line[4*i + 3];
				if (cell != "----") {  // if cell not empty

					if (cell[2] == 'w') {
						_board[row][column].placeFigure(cell[0], true);
					} else if (cell[2] == 'k') {
						_board[row][column].placeFigure(cell[0], false);
					} else { throw invalid_argument("Encountered invalid color while loading board from file!"); }

					// cout << "Placed " << cell << " at " << rowColToAlgebraic(row, column) << endl;
				}
				column++;
			}
			row--;
		}
	}
	input_file.close();
}

bool ChessBoard::kingIsCheck(bool is_white) {
	/* checks if King of the specified color is in check
	 * This is done in the following way: get all possible moves of the 
	 * enemy's figures and try if any figure could capture the King
	 */
	vector<string> enemy_figures;
	vector<string> possible_captures;

	(!is_white) ? enemy_figures = getWhiteFigures() : enemy_figures = getBlackFigures();

	for (auto figure : enemy_figures) {
		possible_captures = getPossibleCaptures(!is_white, figure);
		for (auto capture : possible_captures) {
			if (capture[0] == 'K') {
				return true;
			}
		}
	}
	return false;
}

bool ChessBoard::isValidMove(string notation_input, bool is_white) {
	/* validates a move. e.g. moving a King 2 gridpoints returns false
	 * moving a pawn 1 forward returns true
	 *
	 * also checks, if given move would violate any rules (e.g. king suicide)
	 */
	string figure_notation, move_notation, new_figure_notation;
		
	// e.g. if input is "Bf1b5"
	move_notation = fullNotationToTargetPosition(notation_input);  // b5
	figure_notation = fullNotationToOriginalFigure(notation_input);  // Bf1
	new_figure_notation = fullNotationToTargetFigure(notation_input);  // Bb5

	vector<string> figures;  // vector of all figures, e.g. {"pa2", "pb2", ...}
	(is_white) ? figures = getWhiteFigures() : figures = getBlackFigures();

	if (find(figures.begin(), figures.end(), figure_notation) != figures.end()) {  // if figure valid
		Cell& figure = this->getCell(figureToLocation(figure_notation));  // Reference to current cell of figure

		vector<string> possible_moves = getPossibleMoves(figure_notation);

		// if move valid
		if (find(possible_moves.begin(), possible_moves.end(), move_notation) != possible_moves.end()) {
			// check if this move violates rules
			if (simulateMove(notation_input, is_white, false))
				return true;
		} else {
			printError("Figure cannot move to that location!");
		}
	} else {
		printError("Figure not valid!");
	}
	return false;
}

bool ChessBoard::simulateMove(string notation_input, bool is_white, bool verbose=true) {
	/* creates copy of game and checks if a given move violates certain rules, such as "King Suicide"
	 */
	ChessBoard chess_board_copy;
	chess_board_copy._board = this->_board;

	// Reference to current cell of figure
	Cell& figure = chess_board_copy.getCell(figureToLocation(fullNotationToOriginalFigure(notation_input)));

	figure.removeFigure();  // remove figure from current position
	chess_board_copy.position(fullNotationToTargetFigure(notation_input), is_white);  // place figure in new position
	
	// printInfoBox("Simulated Board:");
	// chess_board_copy.print();

	if(chess_board_copy.kingIsCheck(is_white)) {
		if (verbose) printInfoBox("Cannot move, your King is under 'CHECK'!");
		return false;
	} else {
		return true;
	}
}

bool ChessBoard::isCheckmate(bool is_white) {
	/* Check all possible moves, if no moves result in the color not
	 * staying in check, then the other color is the winner and
	 * we have a checkmate!
	 */

	vector<string> figures, possible_moves;
	(is_white) ? figures = getWhiteFigures() : figures = getBlackFigures();

	for (auto figure : figures) {
		possible_moves = getPossibleMoves(figure);
		for (auto move : possible_moves) {
			// cout << "Checking for checkmate with move " << figure + move << endl;
			if(simulateMove(figure + move, is_white, false)) {
				return false;
			}
		}
	}
	return true;
}
