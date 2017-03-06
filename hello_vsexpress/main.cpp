#include <iostream>
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader.h"
#include "camera.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


/* ==== TODO ====

Wireframe toggle - done
FACKING vertices (no index) working in new method - get normals next
Shading with normals
Clean up obj loader to opengl - maybe write your own obj loader and ditch tinyobj
Find how to structure the program to share needed variables i.e. window WIDTH HEIGHT to shader
Automate loading matrices to shader
Get key inputs for camera manipulation

*/



// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// models

const std::string MODEL_PATH = "models/cube.obj";



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

	struct mesh {
		std::vector<glm::vec3>positions;
		std::vector<glm::vec3>normals;
		std::vector<glm::vec2>uvs;
	};

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
	
	//std::cout << "vert size = " << attrib.vertices.size() << std::endl;
	//std::cout << "norm size = " << attrib.normals.size() << std::endl;
	//std::cout << "index size = " << shapes[0].mesh.indices.size() << std::endl;

	
	// load into inefficient vert list
	std::vector<glm::vec3> vertices;
	
	
	/*for (int i = 0; i < 12; i++) {
		vertices.push_back(glm::vec3());
	}*/
	
	
	for (size_t s = 0; s < shapes.size(); s++) {
		int vSize = attrib.vertices.size()-1;
		for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
			// find the index for each vertex
			tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			// remember to multiply by 3 because the index points to float array, not vectors
			glm::vec3 v1 = glm::vec3(attrib.vertices[3 * idx0.vertex_index + 0], attrib.vertices[3 * idx0.vertex_index + 1], attrib.vertices[3 * idx0.vertex_index + 2]);
			glm::vec3 v2 = glm::vec3(attrib.vertices[3 * idx1.vertex_index + 0], attrib.vertices[3 * idx1.vertex_index + 1], attrib.vertices[3 * idx1.vertex_index + 2]);
			glm::vec3 v3 = glm::vec3(attrib.vertices[3 * idx2.vertex_index + 0], attrib.vertices[3 * idx2.vertex_index + 1], attrib.vertices[3 * idx2.vertex_index + 2]);

			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);
		}
	}
	
	
	
	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size()*sizeof(float), &attrib.vertices.front(), GL_STATIC_DRAW);	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	
	// Set stride - amount of data between each vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(sizeof(glm::vec3)));
	//glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
	glBindVertexArray(0); // Unbind VAO



	// Game loop ================================================================
	float time = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Handle input ( from the keycallback function )
		glfwPollEvents();

		// Render
		
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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


		// Draw		
		glBindVertexArray(VAO);
		// SPECIFY NUMBER OF FUCKING VERTS
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		// SPECIFY NUMBER OF INDICES
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
		time += 0.01f;
	}

	// Loop End ======================

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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