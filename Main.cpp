#include "AI.h"
#include "Move.h"
#include "Game.h"
#include "Input.h"
#include "Output.h"
#include <stdlib.h>
#include <time.h>

#ifdef VISION
#include "Vision.h"
const bool vision = true;
#else
#include "Human.h"
const bool vision = false;
#endif

#ifdef VOICE
#include "Voice.h"
const bool voice = true;
#else
#include "Text.h"
const bool voice = false;
#endif

int main(int argc, char *argv [])
{
	srand(time(NULL));
	int finished = false;
	Move turn;
	Game game;
	//Determining if the game will be played with webcam and audio cues
	#ifdef VISION
		Vision input = Vision();
	#else
		Human input = Human();
	#endif
	#ifdef VOICE
		Voice output = Voice();
	#else
		Text output = Text();
	#endif
	AI ai = AI(0);
	int player = 1;
	while (!finished) {
		if (player == 1) {
			//AI is passed the digital version of the game board and determines where the next disk is placed
			turn = ai.makeMove(game.getGameState(), player);
			//If vision is available, the AI determines where to move and asks the player to make the move
			if (vision) {
				output.requestMove(turn.column + 1);
				Move playerTurn = input.getMove(game.getGameState(), player);
				if (playerTurn != turn) {
					//Checking if the player has cheated by placing the disk incorrectly, or made a mistake
					output.badMove();
					return -1;
				}
			}
		}
		else {
			//The player makes his move
			turn = input.getMove(game.getGameState(), player);
			if (turn.player != player) {
				output.badMove();
				return -1;
			}
		}
		if (game.takeTurn(turn)) {
			if (player == 1) {
				player++;
				output.printBoard(game.getGameState());
			}
			else {
				player--;
			}
		}
		//Evaluate whether either the AI or user has won the game
		finished = game.isWin();
	}
	//Print the board after the winning move is made
	if (finished > 1) {
		output.printBoard(game.getGameState());
	}
	//Announce the disk color for the winning player
	output.announceWinner(finished);

	return 0;
}
