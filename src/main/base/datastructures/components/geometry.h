#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "base/datastructures/components/component.h"
#include "base/vulkan/vulkan.h"
#include <vector>

class Geometry: public Component
{
public:
	Geometry();
	~Geometry();

	virtual void init();
	virtual void update();

	std::vector<Vertex> & getVertexVector();
	std::vector<uint32_t> & getIndexVector();

private:
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
};

#endif // GEOMETRY_H
