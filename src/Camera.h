//
// Created by Omeed on 4/25/25.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <constants.h>

enum CameraPos {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
}

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 startPos = glm::vec3(0.0f, 0.0f, 3.0f),
       glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
       float startYaw = -90.0f, float startPitch = 0.0f)
    : position(startPos),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      up(glm::vec3(0.0f)),    // will be set in updateCameraVectors()
      right(glm::vec3(0.0f)), // same here
      worldUp(startUp),
      yaw(startYaw),
      pitch(startPitch),
      movementSpeed(2.5f),
      mouseSensitivity(0.1f),
      zoom(45.0f)
    {
        updateCameraVectors();
    }

    // Get the view matrix
    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    // Update the camera's orientation
    void processKeyboard(std::string direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == "FORWARD")
            position += front * velocity;
        if (direction == "BACKWARD")
            position -= front * velocity;
        if (direction == "LEFT")
            position -= right * velocity;
        if (direction == "RIGHT")
            position += right * velocity;
    }

    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) {
        if (zoom >= 1.0f && zoom <= 45.0f)
            zoom -= yOffset;
        if (zoom <= 1.0f)
            zoom = 1.0f;
        if (zoom >= 45.0f)
            zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        // Update front, right, and up vectors based on yaw and pitch
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);

        // Recalculate right and up vector
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif //CAMERA_H
