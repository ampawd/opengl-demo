#include "camera.h"


Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp):
    mouseSensivity(0.06f),
    fov(45.0f),
    yaw(-90.0f), pitch(0.0f),
    transformMatrix(glm::mat4(1.0f)),
    movementSpeed(10.0f)
{
    position = cameraPosition;
    front = cameraFront;
    up = cameraUp;
    transformMatrix = glm::lookAt(position, front, up);
	this->updateViewSpace();
}

glm::mat4 Camera::getViewMatrix()
{
    transformMatrix = glm::lookAt(position, position + front, up);
    return transformMatrix;
}

void Camera::handleKeyInput(Directions direction, const GLdouble& dt)
{
    if (direction == FORWARD)
    {
        position += front * movementSpeed * (GLfloat)dt;
    }
    if (direction == BACKWARD)
    {
        position -= front * movementSpeed * (GLfloat)dt;
    }
    if (direction == LEFT)
    {
        position -= glm::cross(front, up) * movementSpeed * (GLfloat)dt;
    }
    if (direction == RIGHT)
    {
        position += glm::cross(front, up) * movementSpeed * (GLfloat)dt;
    }

	//position.y = 0;	//	stay on xz plane
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
	this->updateViewSpace();
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

void Camera::updateViewSpace()
{
	glm::vec3 target;
    target.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    target.y = sin(glm::radians(pitch));
    target.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(target);
	right = glm::normalize(cross(front, up));
	up = glm::normalize(cross(right, front));
}
