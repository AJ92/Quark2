#ifndef RENDERER_H
#define RENDERER_H

#include "base/comp/icomponent.h"

class Renderer: public IComponent
{
public:
	Renderer();
	~Renderer();

	virtual void init();
	virtual void update();

private:

};

#endif // RENDERER_H
