#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

#include <GL/glew.h>

class Shader
{
    public:
        explicit Shader(GLint, const std::string&);
        std::string getSource() const;
        GLint getType() const;
        GLuint getID() const;

    private:
        std::string source;
        GLint type;
        GLuint ID;

    protected:
};

#endif // SHADER_H
