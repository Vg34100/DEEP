#pragma once

#include <GL/gl.h>
#include <unistd.h>  
class Image {
public:
	Image(const char* fname);
	~Image();

	int width;
	int height;
	bool isSpriteSheet;
	int rows;
	int cols;
	unsigned char* data;

	bool loadTexture();
	void render(float x, float y, float scale);

	void renderSprite(int row, int col, float x, float y, float scale, bool flip = false);

	void setSpriteSheet(int rows, int cols); // Set the image as a sprite sheet and define its structure


private:
	GLuint texture;
};
