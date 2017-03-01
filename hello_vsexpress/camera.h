#ifndef CAMERA_H
#define CAMERA_H


#include <iostream>

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Camera {
public:

	Camera(const glm::vec3& _position, const glm::vec3& _lookAt);

	~Camera();

	void update();


	glm::mat4 getView() {
		return glm::lookAt(m_position, m_position + m_viewDir, m_up);
	}

	glm::mat4 getProjection() {
		return glm::perspective(45.0f, m_width / m_height, 0.1f, 100.0f);
	}	

	glm::vec3 getPosition() const {
		return m_position;
	}
private:

	glm::vec3 m_viewDir;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_position;

	//double prevMouseX = 0.0;
	//double prevMouseY = 0.0;

	//double curMouseX = 0.0;
	//double curMouseY = 0.0;

	float m_width = 800.0;
	float m_height = 600.0f;
	


};

#endif