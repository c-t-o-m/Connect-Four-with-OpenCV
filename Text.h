#ifndef TEXT_H
#define TEXT_H

#include "Output.h"
#include "GameState.h"
#include <string>
#include <iostream>

class Text: public Output {
public:
	Text() {}
	void printBoard(GameState gameState);
	void badMove();
	void requestMove(int column);
	void announceWinner(int player);
	void visionError();
	void promptMove(int player);
};

#endif
