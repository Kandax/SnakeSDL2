#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <array>


#include <SDL.h>
#include <SDL_image.h>

#include "ElapsedTime.h"
#include "Texture.h"


struct SnakePart {
	int x = -1;
	int y = 0;
	int oldX = -1;
	int oldY = 0;
};


struct Fruit {
	int x = 10;
	int y = 10;
};


enum  Direction {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
};





class Core {
public:
	Core();
	~Core();
	//run program
	void run();
private:
	//initialize  sdl,sdl_image ,window and renderer 
	bool init();
	//loads textures
	bool loadMedia();

	//free memory
	void close();

	//for the game loop
	void events();
	void input();
	void update();
	void render();


	void randomizeFruitPosition();

	void restartGame();


private:
	std::string mWindowTitle;

	const uint8_t WIDTH;
	const uint8_t HEIGHT;

	const uint8_t SIZE_BLOCK;
	const int SNAKE_SIZE;


	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	bool mQuitWindow;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Event mEvent;

	ElapsedTime mET;

	const Uint8* KEYBOARD;
	Direction mDir;


	Texture mTextureGreen;
	Texture mTextureRed;
	Texture mTextureWhite;


	Fruit mFruit;

	bool mAddingParts;
	std::vector<SnakePart> mSnakeParts;

	char *mBoard;
	


};






