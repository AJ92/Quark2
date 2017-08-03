#ifndef RENDERER_H
#define RENDERER_H

#include "base/datastructures/components/component.h"

class Renderer: public Component
{
public:
	Renderer();
	~Renderer();

	virtual void init();
	virtual void update();

private:

};

#endif // RENDERER_H
