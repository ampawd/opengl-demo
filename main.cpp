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
glm::vec3 cameraPosition(0.0, 0.0, 400.0);
glm::vec3 lightPosition(150.0, 245.0f, 400.0f);

Camera camera(cameraPosition, glm::vec3(0.0, 0.0, -20.0), glm::vec3(0.0, 1.0, 0.0));

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
    GLuint shaderProgramScene, shaderProgramLightSource;
    Shader vshader(GL_VERTEX_SHADER, "shaders/vshader");
    Shader fshader_lightings(GL_FRAGMENT_SHADER, "shaders/fshader_lightings");
    Shader fshader_lightsource(GL_FRAGMENT_SHADER, "shaders/fshader_lightsource");
    Shader gshader(GL_GEOMETRY_SHADER, "shaders/gshader");
    shaderProgramScene = shaderManager.buildProgram(vshader, fshader_lightings, gshader);
    shaderProgramLightSource = shaderManager.buildProgram(vshader, fshader_lightsource, gshader);


    //  vertex position, indices
    GLfloat w = 100.0f, h = 100.0f, d = 100.0f; //  world space
    GLfloat vertices1[] =
    {
        //  positions       //   normals        //  textures
        0.0f, 0.0f, d,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     //  front
        w,    0.0f, d,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        w,    h,    d,      0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.0f, h,    d,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

        0.0f, h,    d,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,     //  top
        w,    h,    d,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        w,    h,    0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        0.0f, h,    0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,   0.0f, 0.0f,     //  back
        w,    0.0f, 0.0f,  0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
        w,    h,    0.0f,  0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
        0.0f, h,    0.0f,  0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

        0.0f, 0.0f, d,     0.0f, -1.0f, 0.0f,   0.0f, 0.0f,     //  bottom
        w,    0.0f, d,     0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        w,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

        0.0f, 0.0f, d,     -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     //  left
        0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        0.0f, h,    0.0f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.0f, h,    d,     -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

        w,    0.0f, d,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     //  right
        w,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        w,    h,    0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        w,    h,    d,      1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
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

    shaderManager.use(shaderProgramScene);
    GLuint objectVao, vbo1, ebo1,
    positionLocScene = glGetAttribLocation(shaderProgramScene, "position"),
    texCoordLocScene = glGetAttribLocation(shaderProgramScene, "texCoord"),
    normalLocScene = glGetAttribLocation(shaderProgramScene, "normal"),
    mvpLocScene = glGetUniformLocation(shaderProgramScene, "mvp"),
    modelLocScene = glGetUniformLocation(shaderProgramScene, "model"),
    viewLocScene = glGetUniformLocation(shaderProgramScene, "view"),
    normalMatrixLoc = glGetUniformLocation(shaderProgramScene, "normalMatrix"),
    cameraPositionLocScene = glGetUniformLocation(shaderProgramScene, "cameraPosition"),
    //matAmbientLoc  = glGetUniformLocation(shaderProgramScene, "material.ambient"),
    matDiffuseLoc  = glGetUniformLocation(shaderProgramScene, "material.diffuse"),
    matSpecularLoc = glGetUniformLocation(shaderProgramScene, "material.specular"),
    matShineLoc    = glGetUniformLocation(shaderProgramScene, "material.shininess"),
    lightPositionLoc = glGetUniformLocation(shaderProgramScene, "light.position"),
    lightAmbientLoc  = glGetUniformLocation(shaderProgramScene, "light.ambient"),
    lightDiffuseLoc  = glGetUniformLocation(shaderProgramScene, "light.diffuse"),
    lightSpecularLoc = glGetUniformLocation(shaderProgramScene, "light.specular");

    //glUniform3f(matAmbientLoc,  0.4f, 0.7f, 0.8f);
    //glUniform3f(matDiffuseLoc,  0.4f, 0.7f, 0.8f);
    //glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(matShineLoc,    32.0f);
    glUniform3f(lightAmbientLoc,  0.2f, 0.2f, 0.2f);
    glUniform3f(lightDiffuseLoc,  0.5f, 0.5f, 0.5f);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(cameraPositionLocScene, camera.position.x, camera.position.y, camera.position.z);

    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);
        glGenBuffers(1, &ebo1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(positionLocScene, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(positionLocScene);

        glVertexAttribPointer(normalLocScene, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(normalLocScene);

        glVertexAttribPointer(texCoordLocScene, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoordLocScene);

        glBindBuffer(GL_ARRAY_BUFFER, 0);   //  unbind
    glBindVertexArray(0);   //  unbind


    shaderManager.use(shaderProgramLightSource);
    GLuint positionLocLight = glGetAttribLocation(shaderProgramLightSource, "position"),
           mvpLocLight = glGetUniformLocation(shaderProgramLightSource, "mvp");
    //  light source is cube
    GLuint lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glVertexAttribPointer(positionLocLight, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(positionLocLight);
    glBindVertexArray(0);


    //  wooden box texture
    GLuint woodenBoxTexture;
    glGenTextures(1, &woodenBoxTexture);
    glBindTexture(GL_TEXTURE_2D, woodenBoxTexture);
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


    //  frame texture
    GLuint woodenBoxTextureFrame;
    glGenTextures(1, &woodenBoxTextureFrame);
    glBindTexture(GL_TEXTURE_2D, woodenBoxTextureFrame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	    //  Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //  Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int faceWidth, faceHeight;
    unsigned char* faceImage = SOIL_load_image("images/container2_specular.png", &faceWidth, &faceHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faceImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(faceImage);
    glBindTexture(GL_TEXTURE_2D, 0);


    std::vector<glm::vec3> positions;
    //positions.push_back(glm::vec3(-100.0f, -50.0f, 0.0f));
    positions.push_back(glm::vec3(-2*w, 0.0f, 0.0f));
    positions.push_back(glm::vec3(0.0f, h, -1.5*d));
    positions.push_back(glm::vec3(w, -h, 50.0f));
    positions.push_back(glm::vec3(-2*w, -2*h, -150.0f));
    positions.push_back(glm::vec3(2*w + 50, 0.0f, 0.0f));
    positions.push_back(glm::vec3(-2*w + 150, -1.5*h, -10.0f));

    glm::mat4 projection, view, model, T, Tback, R, S, mvp, pv, freeTranslate, normalMatrix;
    projection = glm::perspective(camera.getZOOM(), WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);
    S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    //  rendering
    GLfloat currentFrame = 0.0f, lastFrame = 0.0f, dt = 0.0f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        projection = glm::perspective(camera.getZOOM(), WINDOW_SIZE.x/WINDOW_SIZE.y, 0.1f, 10000.0f);
        view = camera.getViewMatrix();

        shaderManager.use(shaderProgramScene);
        glUniform3f(cameraPositionLocScene, camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);

        glBindVertexArray(objectVao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, woodenBoxTexture);
            glUniform1i(matDiffuseLoc, 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, woodenBoxTextureFrame);
            glUniform1i(matSpecularLoc, 1);

            Tback = glm::translate(glm::mat4(1.0f), glm::vec3(w/2, h/2, d/2));
            R = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f) );
            T = glm::translate(glm::mat4(1.0f), -1.0f * glm::vec3(w/2, h/2, d/2));

            do_movement(dt);
            pv = projection * view;
            for (size_t i = 0; i < positions.size(); i++)
            {
                freeTranslate = glm::translate(glm::mat4(1.0f), positions[i]);
                model = freeTranslate * Tback * R * T;
                mvp = pv * model;
                normalMatrix = glm::transpose(glm::inverse(model));
                glUniformMatrix4fv(mvpLocScene, 1, GL_FALSE, glm::value_ptr(mvp));
                glUniformMatrix4fv(modelLocScene, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLocScene, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
                glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
            }
        glBindVertexArray(0);


        shaderManager.use(shaderProgramLightSource);
        glBindVertexArray(lightVao);
            freeTranslate = glm::translate(glm::mat4(1.0f), lightPosition);
            model = S * freeTranslate;
            mvp = pv * model;
            glUniformMatrix4fv(mvpLocLight, 1, GL_FALSE, value_ptr(mvp));
            glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &objectVao);
    glDeleteVertexArrays(1, &lightVao);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &ebo1);

    glfwTerminate();
    return EXIT_SUCCESS;
}
