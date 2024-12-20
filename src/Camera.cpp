#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

void Camera::MouseLook(int mouseX, int mouseY){
    glm::vec2 newMousePosition(mouseX, mouseY);

    glm::vec2 mouseDelta = 0.01f*(newMousePosition-m_oldMousePosition);

    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector)) * m_viewDirection;
    
    m_oldMousePosition = newMousePosition;
}


void Camera::MoveForward(float speed){
    m_eyePosition.z -= speed;
}

void Camera::MoveBackward(float speed){
    m_eyePosition.z += speed;
}

void Camera::MoveLeft(float speed){
    m_eyePosition.x -= speed;
}

void Camera::MoveRight(float speed){
    m_eyePosition.x += speed;
}

void Camera::MoveUp(float speed){
    m_eyePosition.y += speed;
}

void Camera::MoveDown(float speed){
    m_eyePosition.y -= speed;
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


Camera::Camera(){
    std::cout << "(Constructor) Created a Camera!\n";
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);

    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);

    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{

    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
