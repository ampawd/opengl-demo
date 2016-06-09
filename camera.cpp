#include "camera.h"


Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTargetPosition, glm::vec3 cameraUp):
    mouseSensivity(0.06f),
    fov(45.0f),
    yaw(-90.0f), pitch(0.0f),
    transformMatrix(glm::mat4(1.0f)),
    movementSpeed(2.0f)
{
    position = cameraPosition;
    targetPosition = cameraTargetPosition;
    up = cameraUp;
    transformMatrix = glm::lookAt(position, targetPosition, up);
}

glm::mat4 Camera::getViewMatrix()
{
    transformMatrix = glm::lookAt(position, position + targetPosition, up);
    return transformMatrix;
}

void Camera::handleKeyInput(Directions direction, const GLdouble& dt)
{
    if (direction == FORWARD)
    {
        position += targetPosition * movementSpeed * (GLfloat)dt;
    }
    if (direction == BACKWARD)
    {
        position -= targetPosition * movementSpeed * (GLfloat)dt;
    }
    if (direction == LEFT)
    {
        position -= glm::cross(targetPosition, up) * movementSpeed * (GLfloat)dt;
    }
    if (direction == RIGHT)
    {
        position += glm::cross(targetPosition, up) * movementSpeed * (GLfloat)dt;
    }
}

void Camera::handleMouseInput(double dx, double dy)
{
    yaw += dx * mouseSensivity;
    pitch += dy * mouseSensivity;
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
    targetPosition = movementSpeed * glm::normalize(target);
}

void Camera::handleMouseScrollInput(double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    {
        fov -= yoffset/20.0f;
    }
    if (fov <= 1.0f)
    {
        fov = 1.0f;
    }
    if (fov >= 45.0f)
    {
        fov = 45.0f;
    }
}

GLfloat Camera::getZOOM() const
{
    return fov;
}
