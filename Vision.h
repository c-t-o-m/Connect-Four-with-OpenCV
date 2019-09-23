#ifndef VISION_H
#define VISION_H

#include "Input.h"
#include "Move.h"
#include "GameState.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

#ifdef VOICE
#include "Voice.h"
#else
#include "Text.h"
#endif


class Vision: public Input {
private:
	struct Piece {
		cv::Point point;
		int player;
	};
	cv::VideoCapture capture;
	void showImg(cv::Mat frame, std::string name);
	bool tryGetGameState(cv::Mat origFrame, GameState &gameState);
	Move findMoveDifference(GameState origState, GameState currentState);
	void thresholdFrame(cv::Mat frame, cv::Mat threshs[3]);
	void appendPieces(cv::Mat thresh, std::vector<Piece> &pieces, int player);
	#ifdef VOICE
		Voice output = Voice();
	#else
		Text output = Text();
	#endif
public:
	Vision();
	Move getMove(GameState origState, int player);
	//Returns moves made using a webcam
};

#endif
