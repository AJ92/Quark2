#include "imaterial.h"
#include <iostream>

IMaterial::IMaterial(const std::string& name) :
	IComponent(Type::Material)
	, mName(name)
{

}

IMaterial::IMaterial(
	const std::string& name, 
	std::shared_ptr<IShader> shader
) : 
	IComponent(Type::Material), 
	mName(name),
	mShader(shader)
{

};

IMaterial::IMaterial(
		const std::string& name, 
		std::shared_ptr<IShader> shader, 
		std::shared_ptr<ITexture> texture
) : 
	IComponent(Type::Material), 
	mName(name),
	mShader(shader)
{
	mTextures.insert({0,texture});
}
