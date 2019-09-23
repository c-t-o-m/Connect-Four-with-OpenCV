#ifndef OUTPUT_H
#define OUTPUT_H

#include "GameState.h"

class Output {
public:
	virtual void printBoard(GameState gameState) = 0;
	virtual void badMove() = 0;
	virtual void requestMove(int column) = 0;
	virtual void announceWinner(int player) = 0;
	virtual void visionError() = 0;
	virtual void promptMove(int player) = 0;
};

#endif
