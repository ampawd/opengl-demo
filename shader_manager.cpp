#include "shader_manager.h"

ShaderManager::ShaderManager() : currentShaderProgram(0)
{
}

GLuint ShaderManager::buildProgram(const Shader& vshaderInstance, const Shader& fhaderInstance, const Shader& ghaderInstance)
{
    if (!compileShader(vshaderInstance))
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
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        showProgramInfoLog(program);
    }

    glDeleteShader(vshaderInstance.getID());
    glDeleteShader(fhaderInstance.getID());

    return program;
}

void ShaderManager::use(GLuint program)
{
    glUseProgram(program);
    currentShaderProgram = program;
}

GLuint ShaderManager::getUsingProgram() const
{
    return currentShaderProgram;
}

bool ShaderManager::compileShader(const Shader& shaderInstance)
{
    GLint compiled;
    const char* source = shaderInstance.getSource().c_str();
    glShaderSource(shaderInstance.getID(), 1, &source, NULL);
    glCompileShader(shaderInstance.getID());
    glGetShaderiv(shaderInstance.getID(), GL_COMPILE_STATUS, &compiled);
    return compiled;
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
