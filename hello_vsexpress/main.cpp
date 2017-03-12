#include <iostream>
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader.h"
#include "camera.h"
#include "model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


/* ==== TODO ====

fix calculating normals for inverted triangles
clean up overall
Contain all your meshing shit into one function - make easy to draw array of objects
Add error handling for no normals in file
Clean up obj loader to opengl - maybe write your own obj loader and ditch tinyobj
Find how to structure the program to share needed variables i.e. window WIDTH HEIGHT to shader
Automate loading matrices to shader
Get key inputs for camera manipulation

// recently finished
model class for easy loading and drawing of multiple models
Wireframe toggle - done
FACKING vertices (no index) working in new method - get normals next - done
Shading with normals - done

*/



// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

glm::vec3 calcNormal(glm::vec3 _v1, glm::vec3 _v2);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// models

const std::string MODEL_PATH = "models/teapot.obj";



// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW ================================================
	
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	// Create Window ================================================================
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	// for when you don't care about order of key presses and want to handle multiple keys at the same time
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	// Init GLEW ============================================================================================
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Init Viewport =======================================================
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Init Shaders ===================================

	Shader ourShader("vertex_shader.vs", "fragment_shader.fs");


	// Init Camera ==================================

	Camera cam = Camera::Camera(glm::vec3(0.0f, 4.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	



	// Set up vertex data (and buffer(s)) and attribute pointers
	// order must be counter clockwise
	
	glEnable(GL_DEPTH_TEST);

	/*
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
	
	
	// new tiny obj loader

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


	std::string err;
	// c_str on std::string allows to be used for C functions (tinyobj is a C lib)
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str());	
	
	if (!err.empty()) { 
		std::cerr << err << std::endl;
	}
	if (!ret) {
		glfwTerminate();
		return 1;
	}

	

	struct vertex {
		glm::vec3 position;
		glm::vec3 normal;
		//glm::vec2 uv;
	};

	// load into inefficient vert list
	//std::vector<glm::vec3> vertices;
	std::vector<vertex> vertices;
	
	
	
	// for each mesh
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
		
			glm::vec3 n1 = glm::vec3(attrib.normals[3 * idx0.normal_index + 0], attrib.normals[3 * idx0.normal_index + 1], attrib.normals[3 * idx0.normal_index + 2]);
			glm::vec3 n2 = glm::vec3(attrib.normals[3 * idx1.normal_index + 0], attrib.normals[3 * idx1.normal_index + 1], attrib.normals[3 * idx1.normal_index + 2]);
			glm::vec3 n3 = glm::vec3(attrib.normals[3 * idx2.normal_index + 0], attrib.normals[3 * idx2.normal_index + 1], attrib.normals[3 * idx2.normal_index + 2]);

			//calcNormal(v1, v2);

			vertex v1, v2, v3;

			v1.position = p1;
			v2.position = p2;
			v3.position = p3;
			v1.normal = n1;
			v2.normal = n2;			
			v3.normal = n3;

			// have to interlace P.x N.x P.y N.y P.z N.z for stride to work - WRONG NO
			// you were confusing triangles with vertices its  V1p V1n V2p V2n etc
			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);
			
			
			

		}
	}
	
	
	
	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size()*sizeof(float), &attrib.vertices.front(), GL_STATIC_DRAW);	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices.front(), GL_STATIC_DRAW);
	
	// Set stride - amount of data between each vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
	glBindVertexArray(0); // Unbind VAO

	*/

	model cube;

	cube.loadObj(MODEL_PATH);

	// Game loop ================================================================
	float time = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Handle input ( from the keycallback function )
		glfwPollEvents();

		// Render
		
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		ourShader.use();

		glm::mat4 model, view, projection;
		
		model = glm::rotate_slow(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
		if (MODEL_PATH == "models/teapot.obj") {
			float scale = 0.02f;
			model = glm::scale(model, glm::vec3(scale,scale,scale));
		}
		view = cam.getView();
		projection = cam.getProjection();

		// find their allocated uniform location (you allocate them first in the vertex shader, then link to this lcoation)
		GLint modelLoc = glGetUniformLocation(ourShader.program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.program, "view");
		GLint projectionLoc = glGetUniformLocation(ourShader.program, "projection");

		// now pass the matrices to these locations
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		cube.draw();
		//// Draw		
		//glBindVertexArray(VAO);
		//// SPECIFY NUMBER OF FUCKING VERTS
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//// SPECIFY NUMBER OF INDICES
		////glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
		time += 0.01f;
	}

	// Loop End ======================

	// Terminate GLFW, clearing any resources allocated by GLFW.
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

bool wireframe = false;

// key press for ->
// key release for <-
// key repeat for constant
// key unknown = any non standard keyboard button e.g. custom media keys (maybe any?)


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_W) {
		switch (action) 
		{
			case GLFW_PRESS:
				std::cout << "W pressed" << std::endl;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case GLFW_RELEASE:
				std::cout << "W released" << std::endl;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case GLFW_REPEAT:
				//std::cout << " W REPEAT" << std::endl;				
				break;
		}
	}
}

glm::vec3 calcNormal(glm::vec3 _v1, glm::vec3 _v2)
{
	return glm::normalize(glm::cross(_v1, _v2));
}

void loadMesh(std::string _filepath, std::vector<glm::vec3> &_vertices) 
{

}