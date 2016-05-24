#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include "shader.h"


class ShaderManager
{
    public:
        ShaderManager();
        GLuint getUsingProgram() const;
        bool buildProgram(const Shader&, const Shader&, const Shader&);
        void use(GLuint);

    private:
        char errorMessage[512];
        GLint programBuilt;
        GLuint usingProgram;
        bool compileShader(const Shader&);
        void showShaderInfoLog(const Shader&);
        void showProgramInfoLog(GLuint);

};

#endif // SHADER_MANAGER_H
