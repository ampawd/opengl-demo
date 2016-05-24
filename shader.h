#ifndef SHADER_H
#define SHADER_H


#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <stdexcept>

#include "GL/glew.h"

enum ShaderTypes {
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER
};

class Shader
{
    public:
        explicit Shader(ShaderTypes, const std::string&);
        const std::string getSource() const;
        ShaderTypes getType() const;
        GLuint getID() const;

    private:
        const std::string source;
        ShaderTypes type;
        GLuint ID;
};

#endif // SHADER_H
