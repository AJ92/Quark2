#ifndef IGEOMETRY_RENDERER_H
#define IGEOMETRY_RENDERER_H

#include "base/comp/icomponent.h"
#include "base/gfx/components/imaterial.h"
#include "base/gfx/components/igeometry.h"
#include <vector>
#include <string>
#include <memory>

class IGeometryRenderer: public IComponent
{
public:
	explicit IGeometryRenderer(const std::string& name);
	IGeometryRenderer(
            const std::string & name, 
            const std::shared_ptr<IMaterial> material,
            const std::shared_ptr<IGeometry> geometry
        );
	virtual ~IGeometryRenderer() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	void setMaterial(const std::shared_ptr<IMaterial> material);
	void setGeometry(const std::shared_ptr<IGeometry> geometry);

	const std::shared_ptr<IMaterial> getMaterial() const;
	const std::shared_ptr<IGeometry> getGeometry() const;

private:
	const std::string mName;
	std::shared_ptr<IMaterial> mMaterial;
    std::shared_ptr<IGeometry> mGeometry;
};

#endif // IGEOMETRY_RENDERER_H
