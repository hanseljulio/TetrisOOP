#pragma once
# ifndef _SETTINGS_H_
# define _SETTINGS_H_

class Settings {
private:
	int screenWidth = 0;
	int screenHeight = 0;
public:
	// Constructor
	Settings(int screenWidth, int screenHeight);
	Settings();


	// Destructor
	~Settings();

	int getScreenWidth() const {
		return screenWidth;
	}

	void setScreenWidth(int screenWidth) {
		this->screenWidth = screenWidth;
	}

	int getScreenHeight() const {
		return screenHeight;
	}

	void setScreenHeight(int screenHeight) {
		this->screenHeight = screenHeight;
	}

};


# endif

