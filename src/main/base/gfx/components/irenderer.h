#ifndef IRENDERER_H
#define IRENDERER_H

#include "base/comp/icomponent.h"

class IRenderer: public IComponent
{
public:
	IRenderer();
	virtual ~IRenderer() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:

};

#endif // IRENDERER_H
