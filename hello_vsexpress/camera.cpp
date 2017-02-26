#include "camera.h"


Camera::Camera(const glm::vec3& _position, const glm::vec3& _viewDir)
{
	m_position = _position;
	m_viewDir = glm::normalize(_viewDir);
	m_right = glm::normalize(glm::cross(m_viewDir, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_up = glm::vec3(0, 1.0f, 0);
}

Camera::~Camera()
{

}

void Camera::update()
{

}