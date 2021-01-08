#pragma once
# ifndef _TETRISGAME_H_
# define _TETRISGAME_H_
# include <iostream>
# include <string>
# include <Windows.h>
# include <thread>
# include <vector>


using namespace std;

class TetrisGame {
public:
	wstring tetrisShapes[7];
	TetrisGame(int screenWidth, int screenHeight, int difficulty);
	~TetrisGame();

	int fieldWidth = 11;
	int fieldHeight = 18;
	int screenWidth = 0;
	int screenHeight = 0;
	int difficulty = 0;

	unsigned char* playingField = nullptr;


	int rotate(int x, int y, int r);
	bool doesPieceFit(int tetromino, int rotation, int posX, int posY);

	int play();


};

# endif