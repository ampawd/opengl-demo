#include "shader.h"

Shader::Shader(GLint _shaderType, const std::string& filepath): type(_shaderType)
{
	source = "";
    if (filepath.size() != 0)
    {
        std::ifstream ifs(filepath);
        std::string line;
        if (!ifs.is_open())
        {
            std::cerr << "Could not read the shader file" << '\n';
        }

        while (!ifs.eof())
        {
            std::getline(ifs, line);
            source.append(line + '\n');
        }
        ifs.close();
    }
    ID = glCreateShader(_shaderType);
}

std::string Shader::getSource() const
{
    return source;
}

GLint Shader::getType() const
{
    return type;
}

GLuint Shader::getID() const
{
    return ID;
}
