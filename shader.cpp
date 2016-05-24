#include "shader.h"

Shader::Shader(ShaderTypes _type, const std::string& filepath): type(_type)
{
    if (filepath.size() != 0)
    {
        std::ifstream ifs(filepath);
        std::string line, source = "";
        if (!ifs.is_open())
        {
            std::cerr << "Could not read the shader file" << '\n';
        }

        while (!ifs.eof())
        {
            std::getline(ifs, line);
            source += line; source += '\n';
        }
        ifs.close();
    }

    ID = glCreateShader(_type);
}

const std::string Shader::getSource() const
{
    return this->source;
}


ShaderTypes Shader::getType() const
{
    return this->type;
}

GLuint Shader::getID() const
{
    return this->ID;
}
