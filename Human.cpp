#include "Human.h"

Move Human::getMove(GameState gameState, int player) {
	Move turn = Move();
	turn.player = player;
	output.promptMove(player);
	std::cin >> turn.column;
	std::cin.clear();
	std::cin.ignore(1000, '\n');
	std::cout << std::endl;
	--turn.column;
	return turn;
}