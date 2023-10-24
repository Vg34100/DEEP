#include "image.h"
#include <iostream>
#include <GL/glu.h> // Include this header

unsigned char* addAlphaChannel(unsigned char* data, int width, int height) {
	int newSize = width * height * 4;
	unsigned char* newData = new unsigned char[newSize];
	unsigned char tr = data[0];  // Red component of transparent color
	unsigned char tg = data[1];  // Green component
	unsigned char tb = data[2];  // Blue component
	int j = 0;
	for (int i = 0; i < width * height * 3; i+=3) {
		newData[j] = data[i];     // R
		newData[j+1] = data[i+1]; // G
		newData[j+2] = data[i+2]; // B
		// If RGB equals the transparent color, set alpha to 0. Otherwise, 255.
		newData[j+3] = (data[i] == tr && data[i+1] == tg && data[i+2] == tb) ? 0 : 255;
		j += 4;
	}
	return newData;
}


/* More or less from Walk.cpp*/
Image::Image(const char* fname) : data(nullptr), texture(0) {
	if (!fname || fname[0] == '\0')
		return;

	char ppmname[80];
	sprintf(ppmname, "%s.ppm", fname);

	char ts[100];
	sprintf(ts, "convert %s %s", fname, ppmname);
	system(ts);

	FILE* fpi = fopen(ppmname, "r");
	if (fpi) {
		char line[200];
		fgets(line, 200, fpi);  // Read PPM header P6
		fgets(line, 200, fpi);  // Get next line
		while (line[0] == '#')  // Skip comments
			fgets(line, 200, fpi);

		sscanf(line, "%d %d", &width, &height); // Read image size
		fgets(line, 200, fpi);  // Read max color value (255)

		int n = width * height * 3;  // Total number of bytes (3 bytes per pixel)
		data = new unsigned char[n];  // Allocate data array
		fread(data, sizeof(unsigned char), n, fpi);  // Read pixel data
		fclose(fpi);
	} else {
		std::cerr << "ERROR: Unable to open " << ppmname << std::endl;
	}
	unlink(ppmname);  // Delete the temporary PPM image
}


Image::~Image() {
	delete[] data;  // Free image data
	if (texture) {
		glDeleteTextures(1, &texture);  // Delete texture
	}
}

void Image::setSpriteSheet(int rows, int cols) {
	this->isSpriteSheet = true;
	this->rows = rows;
	this->cols = cols;
}

bool Image::loadTexture() {
	if (!data)
		return false;

	// Add alpha channel to the image data using the first pixel as the transparent color
	unsigned char* rgbaData = addAlphaChannel(data, width, height);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] rgbaData; // Free the temporary RGBA data

	return true;
}

void Image::render(float x, float y, float scale) {
	if(!texture) return;

	float aspectRatio = static_cast<float>(width) / height;
	float scaledWidth = scale * aspectRatio;
	float scaledHeight = scale;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x - scaledWidth, y - scaledHeight); // Bottom left
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + scaledWidth, y - scaledHeight); // Bottom right
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + scaledWidth, y + scaledHeight); // Top right
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x - scaledWidth, y + scaledHeight); // Top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void Image::renderSprite(int row, int col, float x, float y, float scale, bool flip) {
    if(!texture || !isSpriteSheet) return;

    float frameWidth = static_cast<float>(width) / cols;
    float frameHeight = static_cast<float>(height) / rows;
    float aspectRatio = frameWidth / frameHeight;
    float scaledWidth = scale * aspectRatio;
    float scaledHeight = scale;

    float s1 = static_cast<float>(col) / cols;
    float s2 = static_cast<float>(col + 1) / cols;
    float t1 = static_cast<float>(row) / rows;
    float t2 = static_cast<float>(row + 1) / rows;

    if (flip) {
        std::swap(s1, s2);  // Swap the horizontal texture coordinates
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(s1, t2); glVertex2f(x - scaledWidth, y - scaledHeight); // Bottom left
        glTexCoord2f(s2, t2); glVertex2f(x + scaledWidth, y - scaledHeight); // Bottom right
        glTexCoord2f(s2, t1); glVertex2f(x + scaledWidth, y + scaledHeight); // Top right
        glTexCoord2f(s1, t1); glVertex2f(x - scaledWidth, y + scaledHeight); // Top left
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
