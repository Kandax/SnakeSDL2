#include "Texture.h"

Texture::Texture()
	:mTexture(nullptr)
	, mWidth(0)
	, mHeight(0)
{
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	free();

	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		std::cout << "unable to load image: " << path.c_str() << " SDL_image ERROR: " << IMG_GetError() << std::endl;

	}
	else {
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr) {
			std::cout << "unable to create texture from: " << path.c_str() << " SDL ERROR: " << SDL_GetError() << std::endl;
		}
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);

	}

	mTexture = newTexture;
	return mTexture != nullptr;
}

void Texture::free()
{
	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y)
{
	SDL_Rect renderQuad = {
		x,
		y,
		mWidth,
		mHeight,
	};

	SDL_RenderCopy(renderer, mTexture, nullptr, &renderQuad);


}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
