#include "Core.h"

Core::Core()
	: mWindowTitle("Snake SDL2")
	, WIDTH(30)
	, HEIGHT(30)
	, SIZE_BLOCK(16)
	, SNAKE_SIZE(5)
	, WINDOW_WIDTH(WIDTH*SIZE_BLOCK)
	, WINDOW_HEIGHT(HEIGHT* SIZE_BLOCK)
	, mQuitWindow(false)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mEvent()
	, mET()
	, KEYBOARD(SDL_GetKeyboardState(nullptr))
	, mDir(Direction::RIGHT)
	, mTextureGreen()
	, mTextureRed()
	, mTextureWhite()
	, mFruit()
	, mAddingParts(false)
{
	mBoard = new char[WIDTH * HEIGHT];



	for (int i = 0; i < SNAKE_SIZE; i++) {
		mSnakeParts.push_back(SnakePart());
	}

	mSnakeParts[0].x = 2;

	// Fills board with "e" that mean empty
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			mBoard[x * HEIGHT + y ] = 'e';
		}
	}





}

Core::~Core()
{
	close();
}

bool Core::init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "problem with initing sdl. SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		mWindow = SDL_CreateWindow(
			mWindowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		if (mWindow == nullptr) {
			std::cout << "Problem with creating window. SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

			if (mRenderer == nullptr) {
				std::cout << "renderer could not be created! SDL ERROR: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					std::cout << "sdl_image could not initialize! SDL_IMAGE ERROR: " << IMG_GetError() << std::endl;
					success = false;
				}


			}

		}

	}

	return success;
}

bool Core::loadMedia()
{
	bool success = true;

	if (!mTextureGreen.loadFromFile(mRenderer, "images/green.png")) {
		std::cout << "failed to load green texture" << std::endl;
		success = false;
	}
	if (!mTextureRed.loadFromFile(mRenderer, "images/Red.png")) {
		std::cout << "failed to load Red texture" << std::endl;
		success = false;
	}
	if (!mTextureWhite.loadFromFile(mRenderer, "images/White.png")) {
		std::cout << "failed to load White texture" << std::endl;
		success = false;
	}


	return success;
}

void Core::close()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	mRenderer = nullptr;
	mWindow = nullptr;

	IMG_Quit();
	SDL_Quit();

	delete[] mBoard;
	mBoard = nullptr;

}

void Core::run()
{
	if (!init()) {
		std::cout << "Failed to initialize" << std::endl;
	}
	else {
		if (!loadMedia()) {
			std::cout << "Failed to load media" << std::endl;
		}
		else {
			while (!mQuitWindow) {
				mET.run();
				events();
				input();
				update();
				render();
			}
		}
	}


}

void Core::events()
{
	while (SDL_PollEvent(&mEvent) != 0) {
		if (mEvent.type == SDL_QUIT) {
			mQuitWindow = true;
		}
	}

}

void Core::input()
{
	// Keyboard input
	if (KEYBOARD[SDL_SCANCODE_LEFT] && mDir != Direction::RIGHT) {
		mDir = Direction::LEFT;
		
	}
	if (KEYBOARD[SDL_SCANCODE_RIGHT] && mDir != Direction::LEFT) {
		mDir = Direction::RIGHT;
	}
	if (KEYBOARD[SDL_SCANCODE_UP] && mDir != Direction::DOWN) {
		mDir = Direction::UP;
	}
	if (KEYBOARD[SDL_SCANCODE_DOWN] && mDir != Direction::UP) {
		mDir = Direction::DOWN;
	}

	if (KEYBOARD[SDL_SCANCODE_SPACE]) {
		mAddingParts = true;
	}
	else {
		mAddingParts = false;
	}

}

void Core::update()
{
	static float sTimer1 = 0;
	sTimer1 += 1 * mET.getElapsedTime();
	//std::cout << "sTimer1: " << sTimer1 << std::endl;

	//timer for appearing another snake part
	if (sTimer1 > 0.6) {
		if (mAddingParts) {
			mSnakeParts.push_back(SnakePart());
			randomizeFruitPosition();
			
		}
		sTimer1 = 0;
	}

	//timer for moving snake
	static float sTimer2 = 0;
	sTimer2 += 1 * mET.getElapsedTime();
	
	if (sTimer2 > 0.1) {
		for (Uint32 i = 0; i < mSnakeParts.size(); i++) {
			if (i == 0) {
				// For the snake head 
				// Set old position with new, and move snake
				if (mDir == Direction::LEFT) {
					mSnakeParts[i].oldX = mSnakeParts[i].x;
					mSnakeParts[i].oldY = mSnakeParts[i].y;
					mSnakeParts[i].x -= 1;

				}
				if (mDir == Direction::RIGHT) {
					mSnakeParts[i].oldX = mSnakeParts[i].x;
					mSnakeParts[i].oldY = mSnakeParts[i].y;
					mSnakeParts[i].x += 1;
				}
				if (mDir == Direction::UP) {
					mSnakeParts[i].oldX = mSnakeParts[i].x;
					mSnakeParts[i].oldY = mSnakeParts[i].y;
					mSnakeParts[i].y -= 1;
				}
				if (mDir == Direction::DOWN) {
					mSnakeParts[i].oldX = mSnakeParts[i].x;
					mSnakeParts[i].oldY = mSnakeParts[i].y;
					mSnakeParts[i].y += 1;
				}
			} 
			// For the rest of snake
			// set old position with new, and set position to old position of earlier snake part
			else {
				mSnakeParts[i].oldX = mSnakeParts[i].x;
				mSnakeParts[i].oldY = mSnakeParts[i].y;
				mSnakeParts[i].x = mSnakeParts[i - 1].oldX;
				mSnakeParts[i].y = mSnakeParts[i - 1].oldY;
			}
		}
		sTimer2 = 0;
	}

	// Fills board with "e" that mean empty
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			mBoard[x * HEIGHT + y] = 'e';
		}
	}

	// Fill given block with "s" if snake part is located on him
	for (int i = 0; i < mSnakeParts.size(); i++) {
		if (mSnakeParts[0].x >= 0 && mSnakeParts[0].x < WIDTH && mSnakeParts[0].y >= 0 && mSnakeParts[0].y < HEIGHT)
			mBoard[mSnakeParts[i].x * HEIGHT+ mSnakeParts[i].y ] = 's';
	}
	// Collision detection with fruit
	if (mSnakeParts[0].x == mFruit.x && mSnakeParts[0].y == mFruit.y) {
		mSnakeParts.push_back(SnakePart());
		randomizeFruitPosition();
	}
	// Collision detection with walls
	if (mSnakeParts[0].x >= WIDTH || mSnakeParts[0].x <= -1 || mSnakeParts[0].y >= HEIGHT || mSnakeParts[0].y <= -1) {
		//window.close();
		//std::cout << "Game Over" << std::endl;
		//mQuitWindow = true;
		restartGame();
	}
	// Collision detection with another snake part
	for (int i = 1; i < mSnakeParts.size(); i++) {
		if (mSnakeParts[0].x == mSnakeParts[i].x && mSnakeParts[0].y == mSnakeParts[i].y) {
			//mQuitWindow = true;
			restartGame();
		}
	}
}

void Core::render()
{

	//std::cout << "elapsed time: " << mET.getElapsedTime() << std::endl;
	//std::cout << "used time: " << mET.getTimeUsed() << std::endl;
	//std::cout << "x: " << mSnakeParts[0].x << " y: " << mSnakeParts[0].y << std::endl;

	//std::cout << "dir: " << mDir << std::endl;
	// 
	//clearing screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);
	// Rendering board
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x >= 0 && x <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT) {
				mTextureWhite.render(mRenderer, x * SIZE_BLOCK, y*SIZE_BLOCK);
			}
		}
	}
	// Rendering snake
	for (int i = 0; i < mSnakeParts.size(); i++) {
		if (mSnakeParts[i].x >= 0 && mSnakeParts[i].x <= WINDOW_WIDTH && mSnakeParts[i].y >= 0 && mSnakeParts[i].y <= WINDOW_HEIGHT) {
			mTextureGreen.render(mRenderer, mSnakeParts[i].x * SIZE_BLOCK, mSnakeParts[i].y * SIZE_BLOCK);
		}
	}




	// Rendering fruit
	if (mFruit.x >= 0 && mFruit.x <= WINDOW_WIDTH && mFruit.y >= 0 && mFruit.y <= WINDOW_HEIGHT) {
		mTextureRed.render(mRenderer, mFruit.x * SIZE_BLOCK, mFruit.y * SIZE_BLOCK);
	}
	

	// Updating screen
	SDL_RenderPresent(mRenderer);


}

void Core::randomizeFruitPosition()
{
	mFruit.x = rand() % WIDTH;
	mFruit.y = rand() % HEIGHT;

	// Search for empty block
	while (1) {
		if (mBoard[mFruit.x * HEIGHT + mFruit.y ] == 's') {
			mFruit.x = rand() % WIDTH;
			mFruit.y = rand() % HEIGHT;
		}
		else {
			break;
		}
	}



}

void Core::restartGame()
{
	mSnakeParts.clear();
	for (int i = 0; i < SNAKE_SIZE; i++) {
		mSnakeParts.push_back(SnakePart());
	}

	mSnakeParts[0].x = 2;
	mSnakeParts[0].y = 2;

	mDir = Direction::RIGHT;


	randomizeFruitPosition();

	
}
