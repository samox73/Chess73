#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

string getNChars(char chr, int N) {
	string out = "";
	for (size_t i {0}; i < N; i++) {
		out += chr;
	}
	return out;
}

string getNStrings(string str, int N) {
	string out = "";
	for (size_t i {0}; i < N; i++) {
		out += str;
	}
	return out;
}

void printInfoBoxLine(string line, char border_char, int width, int offset) {
	cout << getNChars(' ', offset) << border_char;
	cout << getNChars(' ', floor((width - line.length() - 2)/2.0));
	cout << line;
	cout << getNChars(' ', ceil((width - line.length() - 2)/2.0));
	cout << border_char << endl;
}

string getNextWordFromLine(string line) {
	string word {""};
	for (size_t i {0}; i < line.length(); i++) {
		if (line[i] != ' ') {
			word += line[i];
		} else { break; }
	}
	return word;
}

void deleteNextWordFromLine(string& line) {
	while (line[0] != ' ') {
		line.erase(0,1);
	}
	line.erase(0,1);
}

string extractNextValidLine(string& line, int width) {
	string out = "", new_line = "";
	if (line.length() < width) {
		out = line;
		line = "";
	} else {
		while(out.length() + getNextWordFromLine(line).length() < width) {
			out += getNextWordFromLine(line) + " ";
			deleteNextWordFromLine(line);
		}
		//line = new_line;
	}
	return out;
}

void printInfoBox(string _, char border_char = '=', int width = 70, int offset = 10) {
	istringstream text(_);
	string line, broken_line;
	
	cout << endl << getNChars(' ', offset) << getNChars(border_char, width) << endl;
	while(getline(text, line)) {
		while(line != "") {  // getNextValidLine returns a string with length width-2 tops and 
							 // deletes this string from line
			broken_line = extractNextValidLine(line, width - 4);
			printInfoBoxLine(broken_line, border_char, width, offset);
		}
	}
	cout << getNChars(' ', offset) << getNChars(border_char, width) << endl << endl;
}

void printError(string message) {
	printInfoBox(message, '!');
}

void printWelcomeScreen() {
	string out = "";
	out += "Welcome to ChessEngine73!\nValid figures are 'K', 'Q', 'R', 'B', 'N' or 'p'!\n";
	out += "Moves must be specified in algebraic notation, i.e. {figure}{column}{row}. Examples are:\n";
	out += " - Pawn from b2 to b3: 'pb2b3'\n - Queen from d1 to d5: Qd1d5";
	printInfoBox(out, '*');
}
