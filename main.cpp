#define GLEW_STATIC
#define EXIT_SUCCESS 0

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

#include "shader_manager.h"
#include "camera.h"

glm::vec2 WINDOW_SIZE(1200, 800);
glm::vec2 lastMousePos(WINDOW_SIZE.x/2.0, WINDOW_SIZE.y/2.0);

Camera camera(glm::vec3(0.0, 0.0, 600.0), glm::vec3(0.0, 0.0, -20.0), glm::vec3(0.0, 1.0, 0.0));

bool keys[1024];
void key_callback(GLFWwindow* window, int, int, int, int);
void do_movement(const GLfloat&);
void mouse_callback(GLFWwindow* window, double, double);
void scroll_callback(GLFWwindow* window, double, double);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    if (keys[GLFW_KEY_W])  //  forward
    {
        camera.handleKeyInput(FORWARD, dt);
    }
    if (keys[GLFW_KEY_S])  //  backward
    {
        camera.handleKeyInput(BACKWARD, dt);
    }
    if (keys[GLFW_KEY_A])  //  left
    {
        camera.handleKeyInput(LEFT, dt);
    }
    if (keys[GLFW_KEY_D])  //  right
    {
        camera.handleKeyInput(RIGHT, dt);
    }
}

void mouse_callback(GLFWwindow* window, double currentMouseX, double currentMouseY)
{
    GLfloat dx = currentMouseX - lastMousePos.x;
    GLfloat dy = lastMousePos.y - currentMouseY;
    lastMousePos.x = currentMouseX;
    lastMousePos.y = currentMouseY;
    camera.handleMouseInput(dx, dy);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.handleMouseScrollInput(xoffset, yoffset);
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
    GLFWwindow* window = glfwCreateWindow((int)WINDOW_SIZE.x, (int)WINDOW_SIZE.y, "Opengl demo", nullptr, nullptr);
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

    glViewport(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y);
    glEnable(GL_DEPTH_TEST);

    //  initialise GLEW
    glewExperimental = GL_TRUE;                     //  stops glew crashing on OSX :-/
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("glewInit failed");
    }
    ShaderManager shaderManager;
    Shader vshader(GL_VERTEX_SHADER, "shaders/vshader");
    Shader fshader(GL_FRAGMENT_SHADER, "shaders/fshader");
    Shader gshader(GL_GEOMETRY_SHADER, "shaders/gshader");
    GLuint shaderProgram = shaderManager.buildProgram(vshader, fshader, gshader);
    shaderManager.use(shaderProgram);



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
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //  Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int brickWidth, brickHeight;
    unsigned char* brickImage = SOIL_load_image("images/container2.png", &brickWidth, &brickHeight, 0, SOIL_LOAD_RGB);
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
    projection = glm::perspective(camera.getZOOM(), WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);

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

            Tback = glm::translate(glm::mat4(1.0f), glm::vec3(w/2, h/2, d/2));
            //R = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f) );
            T = glm::translate(glm::mat4(1.0f), -1.0f * glm::vec3(w/2, h/2, d/2));

            do_movement(dt);
            projection = glm::perspective(camera.getZOOM(), WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);
            view = camera.getViewMatrix();

            pv = projection * view;
            for (size_t i = 0; i < positions.size(); i++)
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
