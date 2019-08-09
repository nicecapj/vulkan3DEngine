﻿#pragma once
#include <GL/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos);
    ~Camera();

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectMatrix();
    glm::vec3 GetCameraPosition();
private:
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::vec3 camPos_;
    GLfloat fov_;
};

