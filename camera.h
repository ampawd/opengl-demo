#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Directions {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
    public:
        Camera(glm::vec3 = glm::vec3(0.0, 0.0, 0.0), glm::vec3 = glm::vec3(0.0, 0.0, -20.0), glm::vec3 = glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 getViewMatrix();
        void handleKeyInput(Directions, const GLfloat&);
        void handleMouseInput(double, double);
        void handleMouseScrollInput(double, double);
        GLfloat getZOOM() const;
        glm::vec3 position;
        glm::vec3 targetPosition;

    private:
        glm::vec3 up;
        GLfloat mouseSensivity;
        GLfloat fov;
        GLfloat yaw;
        GLfloat pitch;
        GLfloat movementSpeed;
        glm::mat4 transformMatrix;

    protected:

};

#endif // CAMERA_H
