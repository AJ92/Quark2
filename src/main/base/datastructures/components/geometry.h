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
	std::vector<Vertex> & getIndexVector();

private:
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
};

#endif // GEOMETRY_H
