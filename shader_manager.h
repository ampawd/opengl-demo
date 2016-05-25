#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "shader.h"

class ShaderManager
{
    public:
        ShaderManager();
        GLuint buildProgram(const Shader&, const Shader&, const Shader&);
        void use(GLuint);
        GLuint getUsingProgram() const;

    private:
        GLuint currentShaderProgram;
        char errorMessage[1024];
        bool compileShader(const Shader&);
        void showShaderInfoLog(const Shader&);
        void showProgramInfoLog(GLuint);

    protected:

};

#endif // SHADER_MANAGER_H
