#ifndef GAME_H
#define GAME_H

#include "Move.h"
#include <iostream>
#include <string>
#include "GameState.h"

#ifdef VOICE
#include "Voice.h"
#else
#include "Text.h"
#endif

const int COLUMN_COUNT = 7;
const int ROW_COUNT = 6;
const int DIAGONAL_COUNT = 12;
const int DIAGONAL_LENGTH = 6;

class Game
{
    public:
        Game();
        //Makes a move and updates board
        bool takeTurn(Move turn);
        //Checks to see if a player has won the game; Returns the winner's ID; returns 3 in event of draw
        int isWin();
        //Returns the current gameState
        GameState getGameState();
    private:
	#ifdef VOICE
	#include "Voice.h"
			Voice output = Voice();
	#else
	#include "Text.h"
			Text output = Text();
	#endif
        int columns[COLUMN_COUNT][ROW_COUNT];
        int rows[ROW_COUNT][COLUMN_COUNT];
        //LDR abbreviates LeftDownRight, array of the board data stored going in the 315 degree direction
        int diagLDR[DIAGONAL_COUNT][DIAGONAL_LENGTH];
        //RDL abbreviates RightDownLeft, array of the board data stored going in the 225 degree direction
        int diagRDL[DIAGONAL_COUNT][DIAGONAL_LENGTH];
};

#endif // GAME_H
