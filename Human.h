#ifndef HUMAN_H
#define HUMAN_H

#include "Input.h"
#include "Move.h"
#include "GameState.h"
#include <iostream>
#include <string>

#ifdef VOICE
#include "Voice.h"
#else
#include "Text.h"
#endif

class Human: public Input {
private:
	#ifdef VOICE
		Voice output = Voice();
	#else
		Text output = Text();
	#endif
public:
	Move getMove(GameState gameState, int player);
	//Gathers move over terminal
};

#endif
