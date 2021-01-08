# include <iostream>
# include <string>
# include <Windows.h>
# include <thread>
# include <vector>
# include "Settings.h"
# include <stdlib.h>
# include "TetrisGame.h"
# include <fstream>

using namespace std;

vector<int> scoreList;


void fileReader() {
	ifstream file("scores.txt");
	int nums;

	while (file >> nums) {
		scoreList.push_back(nums);
	}
	file.close();
}

void fileWriter(vector<int>& scoreList, int result) {
	for (size_t i = 0; i < scoreList.size(); i++) {
		if (scoreList[i] < result) {
			scoreList.pop_back();
			scoreList.insert(scoreList.begin() + i, result);
			break;
		}
		else if (scoreList[i] == result) {
			break;
		}
	}

	// Clear the text file and replace it with the new scoreList values
	ofstream file("scores.txt", std::ofstream::out | std::ofstream::trunc);
	
	for (size_t i = 0; i < scoreList.size(); i++) {
		file << scoreList[i] << endl;
	}

	file.close();

}


int menu() {
	int value;
	cout << "TETRIS" << endl;
	cout << "[1] Play" << endl;
	cout << "[2] High Scores" << endl;
	cout << "[3] Exit" << endl;
	cout << endl;
	cout << "Select an option: ";

	cin >> value;
	
	return value;
}

void playerSettings(Settings screenSize, int &newScreenWidth, int &newScreenHeight) {
	system("CLS");
	int choice = 1;

	while (choice < 2) {
		cout << "SETTINGS" << endl;

		if (newScreenWidth == 0 && newScreenHeight == 0) {
			cout << "Current resolution: " << 120 << "x" << 30 << endl;
		}
		else {
			cout << "Current resolution: " << newScreenWidth << "x" << newScreenHeight << endl;
		}

		
		cout << "[1] Adjust screen resolution" << endl;
		cout << "[2] Exit" << endl;

		cout << "Select an option: ";
		cin >> choice;

		if (choice == 1) {
			system("CLS");
			cout << "Setting other than 120x30 may break the resolution.\n" 
				"Please readjust the screen accordingly.\n" << endl;
			cout << "New screen width: ";
			cin >> newScreenWidth;
			cout << "New screen height: ";
			cin >> newScreenHeight;

			system("CLS");
		}
	}
	
}


int main() {
	int option = 1;
	Settings screenSize;
	fileReader();
	int newScore = 0;

	int newScreenWidth = 0;
	int newScreenHeight = 0;


	while (option < 3) {
		system("CLS");
		option = menu();

		if (option == 1) {
			if (newScreenWidth == 0 && newScreenHeight == 0) {
				screenSize.setScreenWidth(120);
				screenSize.setScreenHeight(30);
			}
			else {
				screenSize.setScreenWidth(newScreenWidth);
				screenSize.setScreenHeight(newScreenHeight);
			}
			int difficulty = 5;
			system("CLS");

			while (difficulty > 3) {
				cout << "Select difficulty\n"
					"[1] Normal - Play like normal. Nothing weird will happen.\n"
					"[2] Hard - The way it's meant to be. Every 5 shapes will increase the speed of the game.\n"
					"[3] Nightmare - Are you sure about this?\n" << endl;
				cout << "Select a choice: ";
				cin >> difficulty;
				cout << endl;

				if (difficulty > 3) {
					cout << "Invalid input - try again" << endl;
					system("pause");
					system("CLS");
				}
			}
			
			if (difficulty == 3) {
				cout << "Please do not take the nightmare difficulty too seriously. It's just added as a joke, cause why not?\n" << endl;
				system("pause");
			}
			
			TetrisGame game(screenSize.getScreenWidth(), screenSize.getScreenHeight(), difficulty);
			newScore = game.play();
			fileWriter(scoreList, newScore);
		
		}
		else if (option == 2) {
			//playerSettings(screenSize, newScreenWidth, newScreenHeight);
			
			cout << endl;
			cout << "HIGH SCORES" << endl;
			for (size_t i = 0; i < scoreList.size(); i++) {
				cout << i + 1 << ".  " << scoreList[i] << endl;
			}
			cout << endl;
			system("pause");

		}
		
		
	}
	
	return 0;
}