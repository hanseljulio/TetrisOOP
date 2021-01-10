# include <iostream>
# include <string>
# include <Windows.h>
# include <thread>
# include <vector>
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
	ofstream file("scores.txt", ofstream::out | ofstream::trunc);
	
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
	cout << "[3] Secret" << endl;
	cout << "[4] Exit" << endl;
	cout << endl;
	cout << "Select an option: ";

	cin >> value;
	
	return value;
}


int main() {
	int option = 1;
	fileReader();
	int newScore = 0;
	bool secret = false;

	while (option < 4) {
		system("CLS");
		option = menu();

		if (option == 1) {
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
			
			TetrisGame game(120, 30, difficulty, secret);
			newScore = game.play();
			fileWriter(scoreList, newScore);
		
		}
		else if (option == 2) {
			cout << endl;
			cout << "HIGH SCORES" << endl;
			for (size_t i = 0; i < scoreList.size(); i++) {
				cout << i + 1 << ".  " << scoreList[i] << endl;
			}
			cout << endl;
			system("pause");
		}
		else if (option == 3) {
			string code;
			cout << endl;
			cout << "Enter secret code: ";
			cin >> code;
			if (code == "fullslav") {
				secret = true;
				cout << "Hello, comrade." << endl;
			}
			else if (code == "normal") {
				secret = false;
				cout << "Goodbye, comrade." << endl;
			}
			else {
				cout << "Invalid code. Try again." << endl;
			}
			cout << endl;
			system("pause");
		}
	}
	
	return 0;
}