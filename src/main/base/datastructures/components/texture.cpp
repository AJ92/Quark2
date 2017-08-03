#include "texture.h"
#include <iostream>

Texture::Texture() :
	Component(Type::Texture)
{
	bool init_done = _init();

	if (init_done) {
		std::cout << "Texture initialized" << std::endl;
	}
	else {
		std::cout << "Texture initialization failed" << std::endl;
	}
}

Texture::Texture(std::string & textureFile) :
	Component(Type::Texture),
	_texture_file(Texture)
{
	bool init_done = _init();

	if (init_done) {
		std::cout << "Texture " << textureFile << " initialized" << std::endl;
	}
	else {
		std::cout << "Texture " << textureFile << " initialization failed" << std::endl;
	}
}

Texture::~Texture()
{

}

void Texture::update() {
	
}

void Texture::init() {
	
}

///////////////////////////////////////////////
//
//		PRIVATE

bool Texture::_init() {
	if (_texture_file.empty()) {
		return false;
	}
	return true;
}

bool Texture::_deint() {
	//TODO: ...
	return true;
}
