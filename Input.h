#ifndef INPUT_H
#define INPUT_H

#include "Move.h"
#include "GameState.h"

class Input {
public:
	virtual Move getMove(GameState gameState, int players) = 0;
};

#endif
