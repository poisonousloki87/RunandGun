#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

bool wireFrame = false;

int selectedColour = 1;

void windowResizeCallBack(GLFWwindow* window, int width, int height);

void processInputs(GLFWwindow* window);

void showFPS(GLFWwindow* window);

void main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Build WINDOW
	GLFWwindow *window = glfwCreateWindow(800, 600, "Run and Gun", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		system("pause");
		return;
	}
	glfwMakeContextCurrent(window);

	//initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD failed to initialise" << endl;
		glfwTerminate();
		system("pause");
		return;
	}

	//set VIEWPORT
	glViewport(0, 0, 800, 600);

	//load images in, flip them
	stbi_set_flip_vertically_on_load(true);

	//window resize
	glfwSetFramebufferSizeCallback(window, windowResizeCallBack);

	//SHADER PROGRAMS --------------------------------------------------------------------------
	Shader shaderProgram1("vertexShader1.glsl", "fragmentShader1.glsl");

	// -----------------------------------------------------------------------------------------

	float rectVertices[] = {
		0.5f, 0.5f, 0.0f, //top right
		0.5f, -0.5f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f //top left
	};
}

void windowResizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void showFPS(GLFWwindow* window) {
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		stringstream ss;
		ss.precision(3);
		ss << fixed << "FPS: " << fps << " Frame Time: " << msPerFrame << "(ms)";

		glfwSetWindowTitle(window, ss.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}