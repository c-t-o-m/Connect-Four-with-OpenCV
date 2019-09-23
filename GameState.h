#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <algorithm>
#include <iterator>

struct GameState {
	int board[6][7] = { {0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0} };
	GameState() {}
	GameState(const GameState &otherState) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				board[i][j] = otherState.board[i][j];
			}
		}
	}
	bool operator==(const GameState& other) {
		for (int i=0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (board[i][j] != other.board[i][j])
					return false;
			}
		}
		return true;
	}
	bool operator!=(const GameState& other) {
		return !(*this == other);
	}
};

#endif
