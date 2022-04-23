#ifndef IGEOMETRY_RENDERER_H
#define IGEOMETRY_RENDERER_H

#include "base/comp/icomponent.h"
#include <vector>
#include <string>

class IGeometryRenderer: public IComponent
{
public:
	explicit IGeometryRenderer(std::string& name);
	virtual ~IGeometryRenderer() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	std::string mName;
};

#endif // IGEOMETRY_RENDERER_H
