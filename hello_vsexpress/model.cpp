#include "model.h"


model::model() {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);	
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_VAO);
}

model::~model() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

glm::vec3 findNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	glm::vec3 edge1 = p2 - p1;
	glm::vec3 edge2 = p3 - p1;
	return glm::cross(edge1, edge2);
}

void model::loadObj(std::string _filepath) {
	m_vertices.clear();
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	// c_str on std::string allows to be used for C functions (tinyobj is a C lib)
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _filepath.c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	// for each separated mesh
	for (size_t s = 0; s < shapes.size(); s++) {
		// for each face
		for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
			// find the index for each vertex
			tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			// remember to multiply by 3 because the index points to float array, not vectors
			glm::vec3 p1 = glm::vec3(attrib.vertices[3 * idx0.vertex_index + 0], attrib.vertices[3 * idx0.vertex_index + 1], attrib.vertices[3 * idx0.vertex_index + 2]);
			glm::vec3 p2 = glm::vec3(attrib.vertices[3 * idx1.vertex_index + 0], attrib.vertices[3 * idx1.vertex_index + 1], attrib.vertices[3 * idx1.vertex_index + 2]);
			glm::vec3 p3 = glm::vec3(attrib.vertices[3 * idx2.vertex_index + 0], attrib.vertices[3 * idx2.vertex_index + 1], attrib.vertices[3 * idx2.vertex_index + 2]);

			glm::vec3 n1, n2, n3;
			if (attrib.normals.size() == 0) {
				n1, n2, n3 = findNormal(p1, p2, p3);
			}
			else {
				n1 = glm::vec3(attrib.normals[3 * idx0.normal_index + 0], attrib.normals[3 * idx0.normal_index + 1], attrib.normals[3 * idx0.normal_index + 2]);
				n2 = glm::vec3(attrib.normals[3 * idx1.normal_index + 0], attrib.normals[3 * idx1.normal_index + 1], attrib.normals[3 * idx1.normal_index + 2]);
				n3 = glm::vec3(attrib.normals[3 * idx2.normal_index + 0], attrib.normals[3 * idx2.normal_index + 1], attrib.normals[3 * idx2.normal_index + 2]);
			}			

			vertex v1, v2, v3;

			v1.position = p1;
			v2.position = p2;
			v3.position = p3;
			v1.normal = n1;
			v2.normal = n2;
			v3.normal = n3;
			
			m_vertices.push_back(v1);
			m_vertices.push_back(v2);
			m_vertices.push_back(v3);
		}
	}
	// bind up the buffer (buffer is a sub thing of VAO)
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// pass in data
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices.front(), GL_STATIC_DRAW);
	// set size and stride of data (position to location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// set size and stride of data (normal to location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
	glEnableVertexAttribArray(1);

	// data has been passed to the buffer, we can unbind it
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 

}

void model::draw() {
	// bind vao		
	glBindVertexArray(m_VAO);
	// SPECIFY NUMBER OF VERTS
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	// unbind vao
	glBindVertexArray(0);
}

