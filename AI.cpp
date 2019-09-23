#include "AI.h"

AI::AI(int difficulty) {
	if(!setDifficulty(difficulty)) {
		setDifficulty(0);
	}
}

int AI::getDifficulty() {
	return difficulty;
}

bool AI::setDifficulty(int difficulty) {
	if(difficulty >= 0 && difficulty <= 2) {
		this->difficulty = difficulty;
		return true;
	}
	return false;
}

Move AI::makeMove(GameState gameState, int player) {
	//loop scans board and if the board is empty, the AI makes the first move in the middle column
	bool firstMove = true;
	loopDepth=0;
	for (int i = 0; i < COLUMN_COUNT_2; ++i) {
		if (gameState.board[ROW_COUNT_2-1][i] != 0)
			firstMove = false;
	}
	if (firstMove)
		return Move(player, 3);
	
	//if the game has already begun, the AI prioritizes checking "danger spots" where a potential connect 4 is imminent
	else {
		int col = dangerSpot(gameState);
		if(col != -99)
			return Move(player, std::abs(col)-1);
	//if no imminent threats are found, the AI scans the board to make strategic, or random, moves
		else
			return Move(player, thomasAI(gameState));
	}
	return Move(player, rand()%7);
}

double* AI::getOptions(GameState gameState) {
	double *options = new double[7];
	for (int i = 0; i < COLUMN_COUNT_2; ++i) {
		if (gameState.board[0][i] == 0)
			options[i] = 3; //a 3 denotes an available columns
		else
			options[i] = 0;
	}
	return options;
}
//If return is negative then the AI would win, positive beifits player
int AI::dangerSpot(GameState rows, bool future) {
	//This will return the column that the AI must play in to avoid a loss on the next turn.
	int dangerCol = -99;
	int pCol = -99;
	int aiCol = -99;
	for(int i=0; i<ROW_COUNT_2; ++i) {
        for(int j=0; j<COLUMN_COUNT_2; ++j) {
            if(rows.board[i][j] != 0) {
                //Check row for 3 in a row condition
                if(j<=(COLUMN_COUNT_2-3) && (rows.board[i][j]==rows.board[i][j+1] && rows.board[i][j]==rows.board[i][j+2])) {
					//Check to see if the 3 in a row lies on the board's left border and is empty
					if(j!=0 && rows.board[i][j-1]==0) {
						//Check to see if the left side of the 3 in a row is a playable spot.
						if(i==ROW_COUNT_2-1) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)-1);
							}
							pCol= (j+1)-1;
						}
						else if(rows.board[i+1][j-1] != 0) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)-1);
							}
							pCol= (j+1)-1;
						}
					}
					//Check to see if the 3 in a row lies on the board's right border and is empty
					if(j!=COLUMN_COUNT_2-1 && rows.board[i][j+3]==0) {
						//Check to see if the right side of the 3 in a row is a playable spot.
						if(i==ROW_COUNT_2-1) {
							if(rows.board[i][j]==1) {
								aiCol=(-1) * ((j+1)+3);
							}
							pCol= (j+1)+3;
						}
						else if(rows.board[i+1][j+3] != 0) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+3);
							}
							pCol= (j+1)+3;
						}
					}
                }
                //Check column for 3 in a row condition
                if(i<=(ROW_COUNT_2-3) && (rows.board[i][j]==rows.board[i+1][j] && rows.board[i][j]==rows.board[i+2][j])) {
					//Check to see if the 3 in a column lies below the board's top edge and if the space above is empty
					if(i!=0 && rows.board[i-1][j]==0) {
						if(rows.board[i][j]==1) {
							aiCol= (-1) * ((j+1));
						}
						pCol= (j+1);
					}
                }
				//Only check necessary positions for LDR condition
	            if(j<=(COLUMN_COUNT_2-3) && i<=(ROW_COUNT_2-3) && !(i==0 && j==4) && !(i==3 && j==0)) {
					//Check diagonal LDR for 3 in a diagonal condition
	                if(rows.board[i][j]==rows.board[i+1][j+1] && rows.board[i][j]==rows.board[i+2][j+2]) {
						//Check to see if left/up spot is dangerous
						if((i!=0 && j!=0) && rows.board[i-1][j-1]==0 && rows.board[i][j-1]!=0) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)-1);
							}
							pCol= (j+1)-1;
						}
						//Check to see if right/down spot is dangerous
						if((i!=3 && j!=4) && rows.board[i+3][j+3]==0) {
							if(i==2) {
								if(rows.board[i][j]==1) {
									aiCol= (-1) * ((j+1)+3);
								}
								pCol= (j+1)+3;
							}
							else if(rows.board[i+4][j+3]==0) {
								if(rows.board[i][j]==1) {
									aiCol= (-1) * ((j+1)+3);
								}
								pCol= (j+1)+3;
							}
						}
	                }
				}
				//Only check necessary positions for RDL condition
				if(j>=(COLUMN_COUNT_2-5) && i<=(ROW_COUNT_2-3) && !(i==0 && j==2) && !(i==3 && j==COLUMN_COUNT_2-1)) {
					//Check diagonal RDL for 3 in a diagonal condition
	                if(rows.board[i][j]==rows.board[i+1][j-1] && rows.board[i][j]==rows.board[i+2][j-2]) {
						//Check to see if right/up spot is dangerous
						if(i!=0 && j!=COLUMN_COUNT_2-1 && rows.board[i-1][j+1]==0 && rows.board[i][j+1]!=0) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+1);
							}
							pCol= (j+1)+1;
						}
						//Check to see if left/down spot is dangerous
						if((i!=3 && j!=2) && rows.board[i+3][j-3]==0) {
							if(i==2) {
								if(rows.board[i][j]==1) {
									aiCol= (-1) * ((j+1)-3);
								}
								pCol= (j+1)-3;
							}
							else if(rows.board[i+4][j-3]==0) {
								if(rows.board[i][j]==1) {
									aiCol= (-1) * ((j+1)-3);
								}
								pCol= (j+1)-3;
							}
						}
	                }
				}
				//Check for danger spots in holes in rows
				if(j<=(COLUMN_COUNT_2-4)) {
					//Check for 2 in a row followed by open then occupied spot in format xx_x
					if(rows.board[i][j]==rows.board[i][j+1] && rows.board[i][j+2]==0 && rows.board[i][j]==rows.board[i][j+3]) {
						if(i==ROW_COUNT_2-1) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+2);
							}
							pCol= (j+1)+2;
						}
						else if(rows.board[i+1][j+2] !=0 ) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+2);
							}
                    		pCol= (j+1)+2;
						}
					}
					//Check for 2 in a row followed by open then occupied spot in format x_xx
					if(rows.board[i][j]==rows.board[i][j+2] && rows.board[i][j+1]==0 && rows.board[i][j]==rows.board[i][j+3]) {
						if(i==ROW_COUNT_2-1) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+1);
							}
							pCol= (j+1)+1;
						}
						else if(rows.board[i+1][j+1] !=0 ) {
							if(rows.board[i][j]==1) {
								aiCol= (-1) * ((j+1)+1);
							}
                    		pCol= (j+1)+1;
						}
					}
                }
				//Check for danger spots in holes in diagonals LDR
				if(i<ROW_COUNT_2-3 && j<COLUMN_COUNT_2-3) {
					//Check for xx_x holes
					if(rows.board[i][j]==rows.board[i+1][j+1] && rows.board[i+2][j+2]==0 && rows.board[i][j]==rows.board[i+3][j+3] && rows.board[i+3][j+2]!=0) {
						if(rows.board[i][j]==1) {
							aiCol= (-1) * ((j+1)+2);
						}
						pCol= (j+1)+2;
					}
					//Check for x_xx holes
					if(rows.board[i][j]==rows.board[i+2][j+2] && rows.board[i+1][j+1]==0 && rows.board[i][j]==rows.board[i+3][j+3] && rows.board[i+2][j+1]!=0) {
						if(rows.board[i][j]==1) {
							aiCol= (-1) * ((j+1)+1);
						}
						pCol= (j+1)+1;
					}
				}
				//Check for danger spots in holes in diagonals RDL
				if(i<ROW_COUNT_2-3 && j>COLUMN_COUNT_2-5) {
					//Check for xx_x holes
					if(rows.board[i][j]==rows.board[i+1][j-1] && rows.board[i+2][j-2]==0 && rows.board[i][j]==rows.board[i+3][j-3] && rows.board[i+3][j-2]!=0) {
						if(rows.board[i][j]==1) {
							aiCol= (-1) * ((j+1)-2);
						}
						pCol= (j+1)-2;
					}
					//Check for x_xx holes
					if(rows.board[i][j]==rows.board[i+2][j-2] && rows.board[i+1][j-1]==0 && rows.board[i][j]==rows.board[i+3][j-3] && rows.board[i+2][j-1]!=0) {
						if(rows.board[i][j]==1) {
							aiCol= (-1) * ((j+1)-1);
						}
						pCol= (j+1)-1;
					}
				}
			}
        }
    }
	if(!future && aiCol!=-99) {
		return aiCol;
	}
	else if(!future) {
		return pCol;
	}
	else if(future) {
		return pCol;
	}
	return dangerCol;
}

int AI::thomasAI(GameState rows) { // 0 is nothing, 1 is AI, 2 is player, red is AI, yellow is player

	// The AI chooses a random column (1-7) if the player is not about to win. Or if the AI is not about to win.

	double *availableColumns = getOptions(rows);
	int randCol;
	do {
		randCol = rand() % 7;
	} while (availableColumns[randCol] != 3);
	delete availableColumns;

	// ********************************
	// OFFENSIVES ACTIONS  GO UNDER HERE
	//

	// OFFENSIVE DIAGONAL MOVES GO UNDER HERE *************

	// CASE 19: This diagonal will have the AI make offensive diagonal moves, backslash, the rightmost piece :: DONE

	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 0 && rows.board[row - 1][col - 1] == 1 && rows.board[row - 2][col - 2] == 1 && rows.board[row - 3][col - 3] == 1)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return col;
				}
				else if (row == 5)
				{
					return col;
				}

			}

		}
	}

	// Hard Coding Diagonal Case 20
	// This diagonal will have the AI make offensive diagonal moves, backslash, the x.x._.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col - 1] == 0 && rows.board[row - 2][col - 2] == 1 && rows.board[row - 3][col - 3] == 1)
			{
				if (rows.board[row][col - 1] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 21
	// This diagonal will have the AI make offense diagonal moves, backslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col - 1] == 1 && rows.board[row - 2][col - 2] == 0 && rows.board[row - 3][col - 3] == 1)
			{
				if (rows.board[row - 1][col - 2] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 22
	// This diagonal will have the ai make offensive diagonal moves, backslash, the _.x.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col - 1] == 1 && rows.board[row - 2][col - 2] == 1 && rows.board[row - 3][col - 3] == 0)
			{
				if (rows.board[row - 2][col - 3] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 23
	// This diagonal will have the ai make offensive diagonal moves, forwardslash, the x.x.x._ piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col + 1] == 1 && rows.board[row - 2][col + 2] == 1 && rows.board[row - 3][col + 3] == 0)
			{
				if (rows.board[row - 2][col + 3] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 24
	// This diagonal will have the ai make offensive diagonal moves, forwardslash, the x.x._.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col + 1] == 1 && rows.board[row - 2][col + 2] == 0 && rows.board[row - 3][col + 3] == 1)
			{
				if (rows.board[row - 1][col + 2] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 25
	// This diagonal will have the ai  make defensive diagonal moves, forwardslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col + 1] == 0 && rows.board[row - 2][col + 2] == 1 && rows.board[row - 3][col + 3] == 1)
			{
				if (rows.board[row][col + 1] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 26
	// This diagonal will have the ai make offensive diagonal moves, forwardslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 0 && rows.board[row - 1][col + 1] == 1 && rows.board[row - 2][col + 2] == 1 && rows.board[row - 3][col + 3] == 1)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return col;
				}
				else if (row == 5)
				{
					return col;
				}
			}

		}
	}

	// CASE 1: AI winning horizontally, right to left _.x.x.x ::DONE
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 1 && rows.board[row][col - 1] == 1 && rows.board[row][col - 2] == 1 && rows.board[row][col - 3] == 0)
			{
				if (row != 5 && rows.board[row + 1][col - 3] != 0)
				{
					return (col - 3);
				}
				else if (row == 5)
				{
					return (col - 3);
				}

			}
		}
	}

	// CASE 2: AI Wins Within Horizontally  right to left   x._.x.x ::DONE
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 1 && rows.board[row][col - 1] == 1 && rows.board[row][col - 2] == 0 && rows.board[row][col - 3] == 1)
			{
				if (row != 5 && rows.board[row + 1][col - 2] != 0)
				{
					return (col - 2);
				}
				else if (row == 5)
				{
					return (col - 2);
				}

			}
		}
	}

	// CASE 3: AI winning horizontally within , right to left x.x._.x :: DONE
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 1 && rows.board[row][col - 1] == 0 && rows.board[row][col - 2] == 1 && rows.board[row][col - 3] == 1)
			{
				if (row != 5 && rows.board[row + 1][col - 1] != 0)
				{
					return (col - 1);
				}
				else if (row == 5)
				{
					return (col - 1);
				}

			}
		}
	}

	// CASE 4: AI winning horizontally within , right to left x.x.x._  :: DONE
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 0 && rows.board[row][col - 1] == 1 && rows.board[row][col - 2] == 1 && rows.board[row][col - 3] == 1)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return (col);
				}
				else if (row == 5)
				{
					return (col);
				}

			}
		}
	}



	// CASE 5: AI Win Vertically
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 0; col <= 6; col++)
		{
			if (rows.board[row][col] == 1 && rows.board[row - 1][col] == 1 && rows.board[row - 2][col] == 1 && rows.board[row - 3][col] == 0)
			{
				return col;
			}
		}
	}


	// ********************************
	// DEFENSIVE ACTIONS GO UNDER HERE
	//


	// CASE 6: AI Protect Vertically
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 0; col <= 6; col++)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col] == 2 && rows.board[row - 2][col] == 2 && rows.board[row - 3][col] == 0)
			{
				return col;
			}
		}
	}


	// CASE 7: AI winning horizontally, right to left _.x.x.x ::
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 2 && rows.board[row][col - 1] == 2 && rows.board[row][col - 2] == 2 && rows.board[row][col - 3] == 0)
			{
				if (row != 5 && rows.board[row + 1][col - 3] != 0)
				{
					return (col - 3);
				}
				else if (row == 5)
				{
					return (col - 3);
				}

			}
		}
	}

	// CASE 8: AI Wins Within Horizontally  right to left   x._.x.x ::
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 2 && rows.board[row][col - 1] == 2 && rows.board[row][col - 2] == 0 && rows.board[row][col - 3] == 2)
			{
				if (row != 5 && rows.board[row + 1][col - 2] != 0)
				{
					return (col - 2);
				}
				else if (row == 5)
				{
					return (col - 2);
				}

			}
		}
	}

	// CASE 9: AI winning horizontally within , right to left x.x._.x ::
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 2 && rows.board[row][col - 1] == 0 && rows.board[row][col - 2] == 2 && rows.board[row][col - 3] == 2)
			{
				if (row != 5 && rows.board[row + 1][col - 1] != 0)
				{
					return (col - 1);
				}
				else if (row == 5)
				{
					return (col - 1);
				}

			}
		}
	}

	// CASE 10: AI protects horizontally within , right to left x.x.x._  ::
	for (int col = 6; col >= 3; col--)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (rows.board[row][col] == 0 && rows.board[row][col - 1] == 2 && rows.board[row][col - 2] == 2 && rows.board[row][col - 3] == 2)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return (col);
				}
				else if (row == 5)
				{
					return (col);
				}

			}
		}
	}
	// 0 is nothing ||| 1 is AI |||  2 is player
	// Hard Coding Diagonal Case 11
	// This diagonal will have the player make defensive diagonal moves, backslash, the rightmost piece :: DONE

	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 0 && rows.board[row - 1][col - 1] == 2 && rows.board[row - 2][col - 2] == 2 && rows.board[row - 3][col - 3] == 2)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return col;
				}
				else if (row == 5)
				{
					return col;
				}

			}

		}
	}

	// Hard Coding Diagonal Case 12
	// This diagonal will have the player make defensive diagonal moves, backslash, the x.x._.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col - 1] == 0 && rows.board[row - 2][col - 2] == 2 && rows.board[row - 3][col - 3] == 2)
			{
				if (rows.board[row][col - 1] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 13
	// This diagonal will have the player make defensive diagonal moves, backslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col - 1] == 2 && rows.board[row - 2][col - 2] == 0 && rows.board[row - 3][col - 3] == 2)
			{
				if (rows.board[row - 1][col - 2] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 14
	// This diagonal will have the player make defensive diagonal moves, backslash, the _.x.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 6; col >= 3; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col - 1] == 2 && rows.board[row - 2][col - 2] == 2 && rows.board[row - 3][col - 3] == 0)
			{
				if (rows.board[row - 2][col - 3] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 15
	// This diagonal will have the player make defensive diagonal moves, forwardslash, the x.x.x._ piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col + 1] == 2 && rows.board[row - 2][col + 2] == 2 && rows.board[row - 3][col + 3] == 0)
			{
				if (rows.board[row - 2][col + 3] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 16
	// This diagonal will have the player make defensive diagonal moves, forwardslash, the x.x._.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col + 1] == 2 && rows.board[row - 2][col + 2] == 0 && rows.board[row - 3][col + 3] == 2)
			{
				if (rows.board[row - 1][col + 2] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 17
	// This diagonal will have the player make defensive diagonal moves, forwardslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 2 && rows.board[row - 1][col + 1] == 0 && rows.board[row - 2][col + 2] == 2 && rows.board[row - 3][col + 3] == 2)
			{
				if (rows.board[row][col + 1] != 0)
				{
					return col;
				}
			}

		}
	}

	// Hard Coding Diagonal Case 18
	// This diagonal will have the player make defensive diagonal moves, forwardslash, the x._.x.x piece :: DONE
	for (int row = 5; row >= 3; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (rows.board[row][col] == 0 && rows.board[row - 1][col + 1] == 2 && rows.board[row - 2][col + 2] == 2 && rows.board[row - 3][col + 3] == 2)
			{
				if (row != 5 && rows.board[row + 1][col] != 0)
				{
					return col;
				}
				else if (row == 5)
				{
					return col;
				}
			}

		}
	}
	if(loopDepth>=30) {
		return randCol;
	}
	GameState miniMax(rows);
	int depth=0;
	while(depth<ROW_COUNT_2 && miniMax.board[depth][randCol]==0) {
		depth++;
	}
	miniMax.board[depth-1][randCol] = 1;
	if(dangerSpot(miniMax, true)>0) {
		loopDepth++;
		return thomasAI(rows);
	}
	return randCol;
}
