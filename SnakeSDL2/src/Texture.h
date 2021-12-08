#pragma once
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	void free();

	void render(SDL_Renderer* renderer, int x, int y);

	int getWidth();
	int getHeight();
private:
	SDL_Texture* mTexture;


	int mWidth;
	int mHeight;

};

