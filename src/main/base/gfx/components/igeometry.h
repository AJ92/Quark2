#ifndef IGEOMETRY_H
#define IGEOMETRY_H

#include "base/comp/icomponent.h"
#include "base/gfx/vulkan/vulkan.h"
#include <vector>

class IGeometry: public IComponent
{
public:
	IGeometry();
	virtual ~IGeometry() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	std::vector<Vertex> & getVertexVector();
	std::vector<uint32_t> & getIndexVector();

private:
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
};

#endif // IGEOMETRY_H
