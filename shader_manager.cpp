#include "shader_manager.h"
#include "shader.h"

ShaderManager::ShaderManager(): usingProgram(0), programBuilt(false)
{
}

bool ShaderManager::buildProgram(const Shader& vshaderInstance, const Shader& fhaderInstance, const Shader& ghaderInstance)
{
    if (compileShader(vshaderInstance))
    {
        showShaderInfoLog(vshaderInstance);
    }
    if (!compileShader(fhaderInstance))
    {
        showShaderInfoLog(fhaderInstance);
    }
    if (!compileShader(ghaderInstance))
    {
        showShaderInfoLog(ghaderInstance);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vshaderInstance.getID());
    glAttachShader(program, fhaderInstance.getID());

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &programBuilt);
    if (!programBuilt)
    {
        showProgramInfoLog(program);
    }
    glDeleteShader(vshaderInstance.getID());
    glDeleteShader(fhaderInstance.getID());

    return programBuilt;
}

void ShaderManager::use(GLuint program)
{
    if (programBuilt)
    {
        glUseProgram(getUsingProgram());
    }
}

GLuint ShaderManager::getUsingProgram() const
{
    return usingProgram;
}

bool ShaderManager::compileShader(const Shader& shaderInstance)
{
    const char* source = shaderInstance.getSource().c_str();
    glShaderSource(shaderInstance.getID(), 1, &source, NULL);
    glCompileShader(shaderInstance.getID());
    glGetShaderiv(shaderInstance.getID(), GL_COMPILE_STATUS, &programBuilt);
    return programBuilt;
}

void ShaderManager::showShaderInfoLog(const Shader& shaderInstance)
{
    glGetShaderInfoLog(shaderInstance.getID(), 512, NULL, errorMessage);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << errorMessage << std::endl;
}

void ShaderManager::showProgramInfoLog(GLuint program)
{
    glGetProgramInfoLog(program, 512, NULL, errorMessage);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorMessage << std::endl;
}
