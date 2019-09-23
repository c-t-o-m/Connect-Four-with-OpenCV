all:
	g++ Main.cpp -std=c++11 -Wall -o Main.out Game.cpp AI.cpp Voice.cpp Human.cpp Text.cpp Vision.cpp
baseGame:
	g++ Main.cpp -std=c++11 -Wall -o Main.out Game.cpp AI.cpp Human.cpp Text.cpp
