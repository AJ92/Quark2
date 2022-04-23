#include "shader.h"

namespace vk_backend {
    Shader::Shader(
        const std::string & name, 
        const std::string & vertexShaderFile, 
        const std::string & fragmentShaderFile
    ) :
        IShader(name, vertexShaderFile, fragmentShaderFile)
    {

    }

    void Shader::init(){

    }

    void Shader::update(){

    }
}
