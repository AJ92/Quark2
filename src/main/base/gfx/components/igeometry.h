#ifndef IGEOMETRY_H
#define IGEOMETRY_H

#include "base/comp/icomponent.h"
#include <vector>
#include <string>

class IGeometry: public IComponent
{
public:
	explicit IGeometry(const std::string& name);
	virtual ~IGeometry() = default;

	virtual void init() = 0;
	virtual void update() = 0;

private:
	const std::string mName;
};

#endif // IGEOMETRY_H
