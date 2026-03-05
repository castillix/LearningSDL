#pragma once
#include <SDL3/SDL.h>
#include <string>


class LTexture {
public:
	// Initializes texture variables
	LTexture();

	// Cleans up texture variables
	~LTexture();

	//Remove copy constructor
	LTexture( const LTexture& ) = delete;

	//Remove copy assignment
	LTexture& operator=( const LTexture& ) = delete;

	//Remove move constructor
	LTexture( LTexture&& ) = delete;

	//Remove move assignment
	LTexture& operator=( LTexture&& ) = delete;

	// Loads texture from disk
	bool loadFromFile(std::string path);

	// Cleans up texture
	void destroy();

	// Draws texture
	void render(float x, float y);

	// Gets texture attributes
	int getWidth();
	int getHeight();
	bool isLoaded();

private:
	// Contains texture data
	SDL_Texture* mTexture;

	// Texture dimensions
	int mWidth;
	int mHeight;
};