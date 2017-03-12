#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <GL/glew.h>
#include <tiny_obj_loader.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>



struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	//glm::vec2 uv;
};


class model {
public:

	model();
	~model();
	
	std::vector<vertex> m_vertices;
	void draw();
	void loadObj(std::string _filepath);
	

private:

	GLuint m_VAO;
	GLuint m_VBO;
	
	

};


#endif