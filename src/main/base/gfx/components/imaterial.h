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
	explicit IMaterial(std::string& name);
	virtual ~IMaterial() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	std::string mName;	
	std::shared_ptr<IShader> mShader;
	std::map<int, std::shared_ptr<ITexture> > mTextures;
};

#endif // IMATERIAL_H
