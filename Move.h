#ifndef MOVE_H
#define MOVE_H

struct Move {
	int player;
	int column;
	Move() : player(-1), column(-1) {}
	Move(int player, int column) : player(player), column(column) {}
	bool operator==(const Move& other) {
		return (player == other.player && column == other.column);
	}
	bool operator!=(const Move& other) {
		return !(*this == other);
	}
};

#endif
