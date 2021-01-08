# include <iostream>
# include <string>
# include <Windows.h>
# include <thread>
# include <vector>
# include <stdlib.h>
# include "TetrisGame.h"
# include <MMsystem.h>

using namespace std;

TetrisGame::TetrisGame(int screenWidth, int screenHeight, int difficulty) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->difficulty = difficulty;
	
}

TetrisGame::~TetrisGame() {

}


// Rotate function for tetromino
int TetrisGame::rotate(int x, int y, int r) {
	switch (r % 4) {
		case 0: return y * 4 + x; // 0 degrees
		case 1: return 12 + y - (x * 4); // 90 degrees
		case 2: return 15 - (y * 4) - x; // 180 degrees
		case 3: return 3 - y + (x * 4); // 270 degrees
	}
	return 0;
}

// Collision detection
bool TetrisGame::doesPieceFit(int tetromino, int rotation, int posX, int posY) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// Get index into piece
			int pi = rotate(i, j, rotation);

			// Get index into field
			int fi = (posY + j) * fieldWidth + (posX + i);

			if (posX + i >= 0 && posX + i < fieldWidth) {
				if (posY + j >= 0 && posY + j < fieldHeight) {
					if (tetrisShapes[tetromino][pi] == L'X' && playingField[fi] != 0) {
						return false; // fail on first hit
					}

				}
			}

		}
	}

	return true;
}

int TetrisGame::play() {
	system("CLS");

	PlaySound(TEXT("music\\TetrisMusic.wav"), NULL, SND_ASYNC | SND_LOOP);

	tetrisShapes[0].append(L"..X.");
	tetrisShapes[0].append(L"..X.");
	tetrisShapes[0].append(L"..X.");
	tetrisShapes[0].append(L"..X.");

	tetrisShapes[1].append(L"..X.");
	tetrisShapes[1].append(L".XX.");
	tetrisShapes[1].append(L".X..");
	tetrisShapes[1].append(L"....");

	tetrisShapes[2].append(L".X..");
	tetrisShapes[2].append(L".XX.");
	tetrisShapes[2].append(L"..X.");
	tetrisShapes[2].append(L"....");

	tetrisShapes[3].append(L"....");
	tetrisShapes[3].append(L".XX.");
	tetrisShapes[3].append(L".XX.");
	tetrisShapes[3].append(L"....");

	tetrisShapes[4].append(L"..X.");
	tetrisShapes[4].append(L".XX.");
	tetrisShapes[4].append(L"..X.");
	tetrisShapes[4].append(L"....");

	tetrisShapes[5].append(L".X..");
	tetrisShapes[5].append(L".XX.");
	tetrisShapes[5].append(L".X..");
	tetrisShapes[5].append(L"....");

	tetrisShapes[6].append(L"....");
	tetrisShapes[6].append(L"X...");
	tetrisShapes[6].append(L"X...");
	tetrisShapes[6].append(L"XX..");

	playingField = new unsigned char[fieldWidth * fieldHeight];
	

	// Create boundaries
	for (int i = 0; i < fieldWidth; i++) {
		for (int j = 0; j < fieldHeight; j++) {
			// Remember: default is y * width + x
			// Condition ? result if true : result if false
			playingField[j * fieldWidth + i] = (i == 0 || i == fieldWidth - 1 || j == fieldHeight - 1) ? 9 : 0;

		}
	}

	// Game screen
	wchar_t* screen = new wchar_t[screenWidth * screenHeight]; // Initializes screen
	for (int i = 0; i < screenWidth * screenHeight; i++) {
		screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Game loop
	bool gameOver = false;
	int currentPiece = 0; // tetrisShapes[0], tetrisShapes[1], etc
	int currentRotation = 0;
	int currentX = fieldWidth / 2;
	int currentY = 0;
	int score = 0;
	int speedCount = 0;
	int pieceCount = 0;


	bool bKey[4]; // contains the keys (down, left, right, Z)
	bool rotateHold = false; // Keep holding Z and it will go wild

	// For game timing
	int speed = 20;
	int speedCounter = 0;
	bool forceDown = false;
	vector<int> vLines;

	while (!gameOver) {
		// Game timing
		if (difficulty == 3) {
			this_thread::sleep_for(20ms); // Game tick, nightmare mode
		}
		else {
			this_thread::sleep_for(45ms); // Game tick
		}


		 
		speedCounter++;
		forceDown = (speedCounter == speed);


		// Game input
		for (int key = 0; key < 4; key++) {
			bKey[key] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[key]))) != 0;
		}


		// Game logic
		if (bKey[0]) { // If the user press the right key
			if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY)) { // Does it fit?
				currentX++; // Update currentX
			}
		}

		if (bKey[1]) { // If the user press the left key
			if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY)) { // Does it fit?
				currentX--; // Update currentX
			}
		}

		if (bKey[2]) { // If the user press the down key
			if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) { // Does it fit?
				currentY++; // Update currentY
			}
		}

		if (bKey[3]) { // If the user press the Z key
			if (!rotateHold && doesPieceFit(currentPiece, currentRotation + 1, currentX, currentY)) { // Does it fit?
				rotateHold = true;
				currentRotation++; // Update currentRotation
			}
			else {
				rotateHold = false;
			}
		}

		if (forceDown) {
			// Update difficulty every 5 pieces
			if (difficulty > 1) {
				speedCount = 0;
				pieceCount++;
				if (pieceCount % 5 == 0) {
					if (speed >= 10) {
						speed--;
					}
				}
			}


			if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) { // Does it fit?
				currentY++; // Keep going
			}
			else {
				// Lock the current piece in the field (it's not going to move if it's a the bottom)
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (tetrisShapes[currentPiece][rotate(i, j, currentRotation)] == L'X') {
							playingField[(currentY + j) * fieldWidth + (currentX + i)] = currentPiece + 1;

						}
					}
				}
				

				// Any full horizontal lines?
				for (int j = 0; j < 4; j++) {
					if (currentY + j < fieldHeight - 1) {
						bool line = true;
						for (int i = 1; i < fieldWidth - 1; i++) {
							line &= (playingField[(currentY + j) * fieldWidth + i]) != 0;
						}

						if (line) {
							for (int i = 1; i < fieldWidth - 1; i++) {
								playingField[(currentY + j) * fieldWidth + i] = 8;
							}
							vLines.push_back(currentY + j);
						}

					}
				}
				

				score += 25;
				if (!vLines.empty()) {
					score += (1 << vLines.size()) * 100;
				}


				// Choose next piece
				currentX = fieldWidth / 2;
				currentY = 0;
				currentRotation = 0;
				currentPiece = rand() % 7;
				



				// If it does not fit, it's game over
				gameOver = !doesPieceFit(currentPiece, currentRotation, currentX, currentY);
			}
			speedCounter = 0;
		}


		// Draw field
		for (int i = 0; i < fieldWidth; i++) {
			for (int j = 0; j < fieldHeight; j++) {
				// Creates screen, but move by + 2 to make it look better
				screen[(j + 2) * screenWidth + (i + 2)] = L" ABCDEFG=|"[playingField[j * fieldWidth + i]];
			}
		}

		// Draw current piece
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (tetrisShapes[currentPiece][rotate(i, j, currentRotation)] == L'X') {
					screen[(currentY + j + 2) * screenWidth + (currentX + i + 2)] = currentPiece + 65;

				}
			}
		}

		// Animate line completion
		if (!vLines.empty()) {
			// Display Frames (to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(300ms);
			for (auto& v : vLines) {
				for (int i = 1; i < fieldWidth - 1; i++)
				{
					for (int j = v; j > 0; j--) {
						playingField[j * fieldWidth + i] = playingField[(j - 1) * fieldWidth + i];
					}
					playingField[i] = 0;
				}

			}

			//mciSendString("play mp3", NULL, 0, NULL);
			
			vLines.clear();
			//PlaySound(TEXT("music\\LineClear.wav"), NULL, SND_SYNC);
			//PlaySound(TEXT("music\\TetrisMusic.wav"), NULL, SND_ASYNC | SND_LOOP);
		}

		// Display score
		swprintf_s(&screen[2 * screenWidth + fieldWidth + 6], 16, L"SCORE: %8d", score);

		//swprintf_s(&screen[6 * screenWidth + fieldWidth + 6], 16, L"NEXT: %8d", currentPiece);

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

	}

	CloseHandle(hConsole);
	cout << "GAME OVER" << endl;
	cout << "Score: " << score << endl;
	cout << endl;
	system("pause"); // Press any key to continue - built in C++ function

	
	
	return score;
}
