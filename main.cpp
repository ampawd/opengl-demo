#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>


#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "shader_manager.h"

#define EXIT_SUCCESS 0


glm::vec2 WINDOW_SIZE(1200, 800);
GLuint shaderProgram;
GLfloat mixValue = 0.0f,
        fov = 45.0f;

glm::vec3 cameraPos(0.0, 0.0, 800.0),
     cameraTarget(0.0, 0.0, -20.0),
     cameraUp(0.0, 1.0, 0.0);

GLfloat yaw   = -90.0f;
GLfloat pitch =   0.0f;
GLfloat lastX =  1200.0 / 2.0;
GLfloat lastY =  800.0 / 2.0;
bool first = true;
bool keys[1024];


std::string getShaderSource(const std::string&);
void setUpShaders();
void key_callback(GLFWwindow* window, int, int, int, int);
void do_movement(const GLfloat&);
void mouse_callback(GLFWwindow* window, double, double);
void scroll_callback(GLFWwindow* window, double, double);


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

void setUpShaders()
{
    GLuint vshader, fshader;
    GLint success;
    GLchar infoLog[512];
    std::string vshaderStr, fshaderStr;

    vshaderStr = getShaderSource("shaders/vshader");
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

    fshaderStr = getShaderSource("shaders/fshader");
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
    if (action == GLFW_PRESS)
    {
        keys[key] = true;
    }
    if (action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
}

void do_movement(const GLfloat& dt)
{
    GLfloat cameraSpeed = 20.0f * dt;
    if (keys[GLFW_KEY_W])  //  forward
    {
        cameraPos += cameraTarget * cameraSpeed;
    }
    if (keys[GLFW_KEY_S])  //  backward
    {
        cameraPos -= cameraTarget * cameraSpeed;
    }
    if (keys[GLFW_KEY_A])  //  left
    {
        cameraPos -= (glm::cross(cameraTarget, cameraUp)) * cameraSpeed;
    }
    if (keys[GLFW_KEY_D])  //  right
    {
        cameraPos += (glm::cross(cameraTarget, cameraUp)) * cameraSpeed;
    }
    cameraPos.y = 0.0f; //  keep staying on xz plane
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
    if (first)
    {
        lastX = x;
        lastY = y;
        first = false;
    }
    GLfloat sensivity = 0.06f;
    GLfloat dx = x - lastX;
    GLfloat dy = lastY - y;
    lastX = x;
    lastY = y;

    dx *= sensivity;
    dy *= sensivity;

    yaw += dx;
    pitch += dy;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 target;
    target.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    target.y = sin(glm::radians(pitch));
    target.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraTarget = 20.0f * glm::normalize(target);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset/20.0f;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
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
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 4.5?");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowPos(window, 10, 50);

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
        0.0f, 0.0f, d,       //  front
        w,    0.0f, d,
        w,    h,    d,
        0.0f, h,    d,

        0.0f, h,    d,       //  top
        w,    h,    d,
        w,    h,    0.0f,
        0.0f, h,    0.0f,

        0.0f, 0.0f, 0.0f,    //  back
        w,    0.0f, 0.0f,
        w,    h,    0.0f,
        0.0f, h,    0.0f,

        0.0f, 0.0f, d,       //  bottom
        w,    0.0f, d,
        w,    0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, d,       //  left
        0.0f, 0.0f, 0.0f,
        0.0f, h,    0.0f,
        0.0f, h,    d,

        w,    0.0f, d,       //  right
        w,    0.0f, 0.0f,
        w,    h,    0.0f,
        w,    h,    d,
    };

    GLfloat texCoords[] =
    {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
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
    GLsizei indicesCount = sizeof(indices)/sizeof(GLuint);

    GLuint vao1, vbo1, vbo2, ebo1,
    positionLoc = glGetAttribLocation(shaderProgram, "position"),
    texCoordLoc = glGetAttribLocation(shaderProgram, "texCoord"),
    mvpLoc = glGetUniformLocation(shaderProgram, "mvp");

    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
        glGenBuffers(1, &ebo1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glGenBuffers(1, &vbo1);

        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);

        glGenBuffers(1, &vbo2);
        glBindBuffer(GL_ARRAY_BUFFER, vbo2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)0);

        glEnableVertexAttribArray(positionLoc);
        glEnableVertexAttribArray(texCoordLoc);
        glBindBuffer(GL_ARRAY_BUFFER, 0);   //  unbind
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
    unsigned char* brickImage = SOIL_load_image("images/brick.jpg", &brickWidth, &brickHeight, 0, SOIL_LOAD_RGB);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //  Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int faceWidth, faceHeight;
    unsigned char* faceImage = SOIL_load_image("images/t3.jpg", &faceWidth, &faceHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faceImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(faceImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::vector<glm::vec3> positions;
    positions.push_back(glm::vec3(-2*w, 0.0f, 0.0f));
    positions.push_back(glm::vec3(0.0f, h, -1.5*d));
    positions.push_back(glm::vec3(w, -h, 0.0f));
    positions.push_back(glm::vec3(2*w + 50, 0.0f, 0.0f));
    positions.push_back(glm::vec3(-2*w + 150, -1.5*h, -10.0f));

    glm::mat4 projection, view, model, T, Tback, R, S, mvp, pv, freeTranslate;
    projection = glm::perspective(fov, WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    GLfloat currentFrame = 0.0f,
            lastFrame = 0.0f,
            dt = 0.0f;

    //  rendering
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.5f, 0.54f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glBindVertexArray(vao1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, brickTexture);
            glUniform1i(glGetUniformLocation(shaderProgram, "brickTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, faceTexture);
            glUniform1i(glGetUniformLocation(shaderProgram, "faceTexture"), 1);

            glUniform1f(glGetUniformLocation(shaderProgram, "mixValue"), mixValue);

            Tback = glm::translate(glm::mat4(1.0f), glm::vec3(w/2, h/2, d/2));
            R = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f) );
            T = glm::translate(glm::mat4(1.0f), -1.0f * glm::vec3(w/2, h/2, d/2));

            do_movement(dt);
            projection = glm::perspective(fov, WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);

            pv = projection * view;
            for (int i = 0; i < positions.size(); i++)
            {
                freeTranslate = glm::translate(glm::mat4(1.0f), positions[i]);
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
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &ebo1);
    glfwTerminate();
    return EXIT_SUCCESS;
}
