#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>


#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>


#define EXIT_SUCCESS 0

using namespace std;

using namespace glm;


vec2 WINDOW_SIZE(1200, 800);
GLuint vshader, fshader, shaderProgram;
GLfloat mixValue = 0.2f;


void key_callback(GLFWwindow* window, int, int, int, int);

std::string getShaderSource(const std::string&);

void renderScene();

void setUpShaders();

void setUpShaders()
{
    GLint success;
    GLchar infoLog[512];
    std::string vshaderStr, fshaderStr;
    vshaderStr = getShaderSource("vshader");
    const char* vsource = vshaderStr.c_str();
    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vsource, NULL);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    fshaderStr = getShaderSource("fshader");
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsource = fshaderStr.c_str();
    glShaderSource(fshader, 1, &fsource, NULL);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    glUseProgram(shaderProgram);
}

std::string getShaderSource(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    std::string line, source = "";
    if (!ifs.is_open())
    {
        std::cout << "Could not read the shader file" << '\n';
        return source;
    }

    while(!ifs.eof())
    {
        std::getline(ifs, line);
        source += line; source += '\n';
    }

    ifs.close();
    return source;
}

void renderScene()
{
}

int main(int argc, char *argv[])
{
    // initialise GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("glfwInit failed");
        return -1;
    }
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow((int)WINDOW_SIZE.x, (int)WINDOW_SIZE.y, "Window 1", nullptr, nullptr);
    if (!window)
    {
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 4.2?");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    int viewPortW = (int)WINDOW_SIZE.x, viewPortH = (int)WINDOW_SIZE.y;
    glfwGetFramebufferSize(window, &viewPortW, &viewPortH);
    glViewport(0, 0, viewPortW, viewPortH);
    glEnable(GL_DEPTH_TEST);

    //  initialise GLEW
    glewExperimental = GL_TRUE;                     //  stops glew crashing on OSX :-/
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("glewInit failed");
    }

    setUpShaders();


    //  vertex position, indices
    GLfloat w = 100.0f, h = 100.0f, d = 100.0f; //  world space
    GLfloat vertices1[] =
    {
        0.0f, 0.0f, d,      0.0f, 0.0f, //  front
        w,    0.0f, d,      1.0f, 0.0f,
        w,    h,    d,      1.0f, 1.0f,
        0.0f, h,    d,      0.0f, 1.0f,

        0.0f, h,    d,      0.0f, 0.0f, //  top
        w,    h,    d,      1.0f, 0.0f,
        w,    h,    0.0f,   1.0f, 1.0f,
        0.0f, h,    0.0f,   0.0f, 1.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, //  back
        w,    0.0f, 0.0f,   1.0f, 0.0f,
        w,    h,    0.0f,   1.0f, 1.0f,
        0.0f, h,    0.0f,   0.0f, 1.0f,

        0.0f, 0.0f, d,      0.0f, 0.0f, //  bottom
        w,    0.0f, d,      1.0f, 0.0f,
        w,    0.0f, 0.0f,   1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f,

        0.0f, 0.0f, d,      0.0f, 0.0f, //  left
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        0.0f, h,    0.0f,   1.0f, 1.0f,
        0.0f, h,    d,      0.0f, 1.0f,

        w,    0.0f, d,      0.0f, 0.0f, //  right
        w,    0.0f, 0.0f,   1.0f, 0.0f,
        w,    h,    0.0f,   1.0f, 1.0f,
        w,    h,    d,      0.0f, 1.0f
    };

    GLuint indices[] =
    {
         // front
         0,  1,  2,
         2,  3,  0,
        // top
         4,  5,  6,
         6,  7,  4,
        // back
         8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20
    };

    GLuint vao1, vbo1, ebo1,
    positionLoc = glGetAttribLocation(shaderProgram, "position"),
    texCoordLoc = glGetAttribLocation(shaderProgram, "texCoord"),
    mvpLoc = glGetUniformLocation(shaderProgram, "mvp");

    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
        glGenBuffers(1, &vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(positionLoc);
        glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoordLoc);

        glGenBuffers(1, &ebo1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);   //  unbind

    //  brick texture
    GLuint brickTexture;
    glGenTextures(1, &brickTexture);
    glBindTexture(GL_TEXTURE_2D, brickTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       //  Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int brickWidth, brickHeight;
    unsigned char* brickImage = SOIL_load_image("brick.jpg", &brickWidth, &brickHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickWidth, brickHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, brickImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(brickImage);
    glBindTexture(GL_TEXTURE_2D, 0);


    //  face texture
    GLuint faceTexture;
    glGenTextures(1, &faceTexture);
    glBindTexture(GL_TEXTURE_2D, faceTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	    //  Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int faceWidth, faceHeight;
    unsigned char* faceImage = SOIL_load_image("t3.jpg", &faceWidth, &faceHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faceImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(faceImage);
    glBindTexture(GL_TEXTURE_2D, 0);


    //  rendering

    vec3 positions[4] =
    {
        vec3(-2*w, 0.0f, 0.0f),
        vec3(0.0f, h, -1.3*d),
        vec3(w, -h, 0.0f),
        vec3(2*w + 100, 0.0f, 0.0f),
    };
    GLsizei indicesCount = sizeof(indices)/sizeof(GLuint);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mat4 projection, view, model, T, Tback, R, S, mvp, pv, freeTranslate;
    projection = perspective(45.0f, WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 2000.0f);
    view = lookAt(vec3(0.0f, 0.0f, 750.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    pv = projection * view;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.5f, 0.54f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, brickTexture);
            glUniform1i(glGetUniformLocation(shaderProgram, "brickTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, faceTexture);
            glUniform1i(glGetUniformLocation(shaderProgram, "faceTexture"), 1);
            glUniform1f(glGetUniformLocation(shaderProgram, "mixValue"), mixValue);

            Tback = translate(mat4(1.0f), vec3(w/2, h/2, d/2));
            R = rotate(mat4(1.0f), (GLfloat)glfwGetTime(), vec3(1.0, 0.0, 0.0));
            T = translate(mat4(1.0f), -1.0f * vec3(w/2, h/2, d/2));

            for (int i = 0; i < 4; i++)
            {
                freeTranslate = translate(mat4(1.0f), -1.0f*positions[i]);
                model = freeTranslate * Tback * R * T;
                mvp = pv * model;
                glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));
                glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
            }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao1);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &ebo1);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_LEFT)
    {
        mixValue -= 0.05;
        if (mixValue < 0.0)
        {
            mixValue = 0.0;
        }
    }
    if (key == GLFW_KEY_RIGHT)
    {
        mixValue += 0.05;
        if (mixValue > 1.0)
        {
            mixValue = 1.0;
        }
    }
}
