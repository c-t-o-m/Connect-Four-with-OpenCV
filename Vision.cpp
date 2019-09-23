#include "Vision.h"

#include <stdlib.h>

Vision::Vision() {
	capture = cv::VideoCapture(0);

	if (!capture.isOpened()) {
		output.visionError();
		throw "Capture Failed";
	}
}

Move Vision::getMove(GameState origState, int player) {
	GameState backTwo = origState, backOne = origState;
	int frameCount = 0;
	while (true) {
		cv::Mat frame;
		capture.read(frame);
		cv::imshow("cam", frame);
		GameState currentState;
		int success = tryGetGameState(frame, currentState);
		cv::waitKey(10);
		if (success) {
			frameCount++;
			//If the last three frames are the same it now counts
			//This way it doesnt count moves as the peices are falling
			if (currentState == backOne && currentState == backTwo) {
				if (currentState != origState) {
					//first 3 frames is before any player moves have been made
					if (frameCount == 3) {
						output.visionError();
						throw "Board Mismatch";
					}
					return findMoveDifference(origState, currentState);
				}
			}
			backTwo = backOne;
			backOne = currentState;
		}
	}
}

void Vision::showImg(cv::Mat frame, std::string name) {
	while (true) {
		cv::imshow(name, frame);
		if (cv::waitKey(10))
			break;
	}
}

void Vision::thresholdFrame(cv::Mat origFrame, cv::Mat threshs[3]) {
	//Thresholds the frame into 3 parts, Black (no peices), Red, and Yellow
	cv::Mat frame, blackThresh, blueThresh, redThresh, otherRedThresh, yellowThresh, mask;
	cv::resize(origFrame, frame, cv::Size(), 0.5, 0.5);
	cv::GaussianBlur(frame, frame, cv::Size(15, 15), 2, 2);
	cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
	//Numbers found experimentally
	cv::inRange(frame, cv::Scalar(101, 75, 40), cv::Scalar(150, 255, 255), blueThresh);
	cv::inRange(frame, cv::Scalar(151, 130, 40), cv::Scalar(255, 255, 255), redThresh);
	cv::inRange(frame, cv::Scalar(0, 130, 40), cv::Scalar(20, 255, 255), otherRedThresh);
	cv::inRange(frame, cv::Scalar(21, 130, 40), cv::Scalar(100, 255, 255), yellowThresh);
	//Makes a mask by threshing for blue and filling in all the edges
	blueThresh.copyTo(mask);
	cv::floodFill(mask, cv::Point(0, 0), cv::Scalar(125));
	cv::Scalar colour = mask.at<uchar>(cv::Point(frame.cols / 2, frame.rows - 1));
	if (colour.val[0] == 0)
		cv::floodFill(mask, cv::Point(frame.cols / 2, frame.rows - 1), cv::Scalar(125));
	cv::threshold(mask, mask, 150, 0, cv::THRESH_TOZERO_INV);
	cv::threshold(mask, mask, 100, 255, cv::THRESH_BINARY_INV);
	cv::bitwise_or(redThresh, otherRedThresh, redThresh);
	//Only includes parts of the red, yellow thresh surrounded by blue
	cv::bitwise_and(blueThresh, mask, blueThresh);
	cv::bitwise_and(redThresh, mask, redThresh);
	cv::bitwise_and(yellowThresh, mask, yellowThresh);

	//Makes the black thresh by finding whats not blue yellow or red
	cv::Mat combinedThresh(blueThresh.rows, blueThresh.cols, CV_8UC3);
	cv::Mat in [] = { blueThresh, yellowThresh, redThresh };
	int from_to [] = { 0,0, 1,1, 2,2 };
	mixChannels(in, 3, &combinedThresh, 1, from_to, 3);
	cv::cvtColor(combinedThresh, combinedThresh, CV_BGR2GRAY);
	cv::GaussianBlur(combinedThresh, combinedThresh, cv::Size(15, 15), 2, 2);

	cv::threshold(combinedThresh, blackThresh, 5, 255, cv::THRESH_BINARY_INV);
	cv::bitwise_and(mask, blackThresh, blackThresh);
	threshs[0] = blackThresh;
	threshs[1] = redThresh;
	threshs[2] = yellowThresh;
}

void Vision::appendPieces(cv::Mat thresh, std::vector<Piece> &pieces, int player) {
	//Takes a threshold, finds the contour centers, and appends it as a peice
	cv::Mat canny;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Canny(thresh, canny, 100, 100 * 2, 3);
	findContours(canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for (int i = 0; i < contours.size(); i++) {
		if (cv::contourArea(contours[i], true) > 15) {
			cv::RotatedRect rect = cv::fitEllipse(cv::Mat(contours[i]));
			Piece piece;
			piece.point = rect.center;
			piece.player = player;
			pieces.push_back(piece);
		}
	}
}

bool Vision::tryGetGameState(cv::Mat origFrame, GameState &gameState) {
	cv::Mat threshs[3];
	std::vector<Piece> pieces;
	thresholdFrame(origFrame, threshs);
	showImg(threshs[0], "Dots");
	for (int i=0; i < 3; i++)
		appendPieces(threshs[i], pieces, i);
	if (pieces.size() != 42)
		return false;

	std::sort(pieces.begin(), pieces.end(),
		[](const Piece& piece1, const Piece& piece2) {
			return (piece1.point.y < piece2.point.y);
		}
	);
	for (int i = 0; i < 6; i++) {
		std::vector<Piece> row(pieces.begin() + 7*i, pieces.begin() + 7*(i+1));
		std::sort(row.begin(), row.end(),
			[](const Piece& piece1, const Piece& piece2) {
				return (piece1.point.x < piece2.point.x);
			}
		);
		for (int j = 0; j < 7; j++) {
			gameState.board[i][j] = row.at(j).player;
		}
	}

	return true;
}

Move Vision::findMoveDifference(GameState origState, GameState currentState) {
	Move move = Move();
	bool alreadyFound = false;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (origState.board[i][j] != currentState.board[i][j]) {
				if (alreadyFound) {
					output.visionError();
					throw "Multiple Moves";
				}
				alreadyFound = true;
				move.player = currentState.board[i][j];
				move.column = j;
			}
		}
	}
	return move;
}
