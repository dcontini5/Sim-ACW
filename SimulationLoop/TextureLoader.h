#pragma once
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	static GLuint LoadBMP(const char * imagepath);
};

