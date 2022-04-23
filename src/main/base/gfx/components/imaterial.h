#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "base/comp/icomponent.h"
#include "base/gfx/components/ishader.h"
#include "base/gfx/components/itexture.h"
#include <memory>
#include <map>

class IMaterial: public IComponent
{
public:
	explicit IMaterial(const std::string& name);
	IMaterial(const std::string& name, std::shared_ptr<IShader> shader);
	IMaterial(
		const std::string& name, 
		std::shared_ptr<IShader> shader, 
		std::shared_ptr<ITexture> texture
	);
	virtual ~IMaterial() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	const std::string mName;	
	std::shared_ptr<IShader> mShader;
	std::map<int, std::shared_ptr<ITexture> > mTextures;
};

#endif // IMATERIAL_H
