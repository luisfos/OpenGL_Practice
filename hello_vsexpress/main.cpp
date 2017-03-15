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

fix calculating normals for inverted triangles - not normals its z-fighting in the mesh
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
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

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

		// Draw objects
		cube.draw();


		// Swap the screen buffers
		glfwSwapBuffers(window);
		time += 0.01f;
	}

	// Loop End ======================

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

bool wireframe = false;

// key press for ->|
// key release for <-|
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
