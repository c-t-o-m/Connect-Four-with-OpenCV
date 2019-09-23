#include "Voice.h"

void Voice::printBoard(GameState gameState) {
	text.printBoard(gameState);
}

void Voice::badMove() {
	text.badMove();
	PlaySound(TEXT("audio/badmove.wav"), NULL, SND_ALIAS | SND_APPLICATION | SND_ASYNC);
}

void Voice::requestMove(int column) {
	text.requestMove(column);
	const char *filename;
	switch (column) {
	case 1: filename = "audio/pleasemoveincolumn1.wav";
		break;
	case 2: filename = "audio/pleasemoveincolumn2.wav";
		break;
	case 3: filename = "audio/pleasemoveincolumn3.wav";
		break;
	case 4: filename = "audio/pleasemoveincolumn4.wav";
		break;
	case 5: filename = "audio/pleasemoveincolumn5.wav";
		break;
	case 6: filename = "audio/pleasemoveincolumn6.wav";
		break;
	case 7: filename = "audio/pleasemoveincolumn7.wav";
		break;
	default: filename = "audio/badmove.wav";
		break;
	}
	PlaySound(TEXT(filename), NULL, SND_ALIAS | SND_APPLICATION | SND_ASYNC);

}

void Voice::announceWinner(int player) {
	if (player == 1) {
		std::cout << "Player 1 has won the game!" << std::endl;
	}
	else if (player == 2) {
		std::cout << "Player 2 has won the game!" << std::endl;
	}
	else {
		std::cout << "Stalemate!" << std::endl;
	}
	const char *filename;
	if (player == 1)
		filename = ("audio/redwins.wav");
	else if(player == 2)
		filename = ("audio/yellowwins.wav");
	else {
		filename = ("audio/stalemate.wav");
	}
	PlaySound(TEXT(filename), NULL, SND_ALIAS | SND_APPLICATION);
}

void Voice::visionError() {
	text.visionError();
	PlaySound(TEXT("audio/visionerror.wav"), NULL, SND_ALIAS | SND_APPLICATION);
}

void Voice::promptMove(int player) {
	std::cout << std::endl << "Enter " << ((player == 1) ? "Red" : "Yellow") << " player's move column: ";
	PlaySound(TEXT("audio/promptmove.wav"), NULL, SND_ALIAS | SND_APPLICATION | SND_ASYNC);
}