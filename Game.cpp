#include "Game.h"

Game::Game() {
    for(int i=0; i<COLUMN_COUNT; ++i) {
        for(int j=0; j<ROW_COUNT; ++j) {
            columns[i][j] = 0;
        }
    }
    for(int i=0; i<ROW_COUNT; ++i) {
        for(int j=0; j<COLUMN_COUNT; ++j) {
            rows[i][j] = 0;
        }
    }
    for(int i=0; i<DIAGONAL_COUNT; ++i) {
        for(int j=0; j<DIAGONAL_LENGTH; ++j) {
            diagLDR[i][j] = 0;
            diagRDL[i][j] = 0;
        }
    }
}

bool Game::takeTurn(Move turn) {
	if(turn.column < 0 || turn.column >= COLUMN_COUNT) {
		output.badMove();
        return false;
	}
    int pos = -1;
    if(columns[turn.column][ROW_COUNT-1]==0) {
        pos=ROW_COUNT-1;
    }
    else while(columns[turn.column][pos+1]==0) {
        pos++;
    }
    if(pos==-1) {
		output.badMove();
        return false;
    }
    columns[turn.column][pos] = turn.player;
    rows[pos][turn.column] = turn.player;
    //Funcky formulas to set up diagonals from left to right
    if((turn.column+(ROW_COUNT-pos-1)) < 6) {
        diagLDR[turn.column+(ROW_COUNT-pos-1)][turn.column] = turn.player;
    }
    else {
        diagLDR[turn.column+(ROW_COUNT-pos-1)][pos] = turn.player;
    }
    //Funcky formulas to set up diagonals from right to left
    if((COLUMN_COUNT-turn.column-1)+(ROW_COUNT-pos-1) < 6) {
        diagRDL[(COLUMN_COUNT-turn.column-1)+(ROW_COUNT-pos-1)][COLUMN_COUNT-turn.column-1] = turn.player;
    }
    else {
        diagRDL[(COLUMN_COUNT-turn.column-1)+(ROW_COUNT-pos-1)][pos] = turn.player;
    }
    return true;
}

int Game::isWin() {
    int posCounter=0;
    for(int i=0; i<ROW_COUNT; ++i) {
        for(int j=0; j<COLUMN_COUNT; ++j) {
            if(rows[i][j] != 0) {
                posCounter++;
                //Check row? for win condition
                if(j<=(COLUMN_COUNT-4) && (rows[i][j]==rows[i][j+1] && rows[i][j]==rows[i][j+2] && rows[i][j]==rows[i][j+3])) {
                    return rows[i][j];
                }
                //Check column? for win condition
                else if(i<=(ROW_COUNT-4) && (rows[i][j]==rows[i+1][j] && rows[i][j]==rows[i+2][j] && rows[i][j]==rows[i+3][j])) {
                    return rows[i][j];
                }
            }
            //Check diagonal for win condition
            if(j<=2) {
                if(diagLDR[i+3][j]!=0 && (diagLDR[i+3][j]==diagLDR[i+3][j+1] && diagLDR[i+3][j]==diagLDR[i+3][j+2] && diagLDR[i+3][j]==diagLDR[i+3][j+3])) {
                    return diagLDR[i+3][j];
                }
                if(diagRDL[i+3][j]!=0 && (diagRDL[i+3][j]==diagRDL[i+3][j+1] && diagRDL[i+3][j]==diagRDL[i+3][j+2] && diagRDL[i+3][j]==diagRDL[i+3][j+3])) {
                    return diagRDL[i+3][j];
                }
            }
        }
    }
    if(posCounter==42) {
        return 3;
    }
    return 0;
}

GameState Game::getGameState() {
    GameState gs;
    for(int i=0; i<ROW_COUNT; ++i) {
        for(int j=0; j<COLUMN_COUNT; ++j) {
            gs.board[i][j] = rows[i][j];
        }
    }
    return gs;
}
