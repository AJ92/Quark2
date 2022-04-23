#include "geometry.h"

namespace vk_backend {
    Geometry::Geometry(std::string& name) :
        IGeometry(name)
    {

    }

    Geometry::Geometry(
        std::string& name, 
        const std::vector<Vertex> & vertices,
        const std::vector<uint32_t> & indices
    ) :
        IGeometry(name),
        mVertexVector(vertices),
        mIndexVector(indices)
    {
        
    }

    void Geometry::init(){

    }

    void Geometry::update(){

    }

    const std::vector<Vertex> & Geometry::getVertexVector(){
	    return mVertexVector;
    }
    
    const std::vector<uint32_t> & Geometry::getIndexVector(){
        return mIndexVector;
    }
}
