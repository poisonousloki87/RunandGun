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
	Shader shaderProgram1("vertexShader4.txt", "fragmentShader4.txt");
	// -----------------------------------------------------------------------------------------

	//the vertices order, treating each xyz as an index
	unsigned int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3 // second triangle
	};

	float BackgroundtVertices[] = {
		// positions // colors // texture coords
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left 
	};


	//BACKGROUND 
	// -----------------------------------------------------------------------------------------

	//CREATE VBO TO STORE VERTICES
	unsigned int textureRectVBO;
	glGenBuffers(1, &textureRectVBO);
	//CREATE EBO TO STORE CONNECTION INDEXES 
	unsigned int textureRectEBO;
	glGenBuffers(1, &textureRectEBO);
	//CREATE VAO TO STORE OPERATIONS ON VBO
	unsigned int textureRectVAO;
	glGenVertexArrays(1, &textureRectVAO);
	//to work with this VAO bind it to make it the current one
	glBindVertexArray(textureRectVAO);
	//bind the buffer object to this vao
	glBindBuffer(GL_ARRAY_BUFFER, textureRectVBO);
	//give data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundtVertices), BackgroundtVertices, GL_STATIC_DRAW);

	//bind elemental buffer object to this vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureRectEBO);
	//give indices to ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//tell VAO which part of the VBO is for location = 0 of our vertex shader
	//Position(x,y,z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//onto location = 1
	//Colour(R,G,B)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//last, location = 2
	//Texture Coordinate(S,T)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//unbind our VAO
	glBindVertexArray(0);

	//generate a texture in gpu, return id
	unsigned int texture1ID;
	glGenTextures(1, &texture1ID);
	//we bind the texture to make it the one we're working on
	glBindTexture(GL_TEXTURE_2D, texture1ID);
	//set wrapping options(repeat texture if texture coordinates dont fully cover polygons)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//wrap on the s(x) axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//wraps on the t(y) axis
																 //set filtering options
																 //Suggestion use nearest neighbour for pixel art, use bilinear for pretty much everything else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR(bilinear) or GL_NEAREST for shrinking
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//for stretching

																	 //LOAD UP IMAGE FILE (JPEG FIRST)
	int width, height, numberChannels; //as we load an image, we'll get values from it to fill these in
	unsigned char *image1Data = stbi_load("Assets/wallpaper.png", &width, &height, &numberChannels, 0);
	//if it loaded
	if (image1Data) {
		cout << "Success! Image is " << width << " by " << height << "pixels" << endl;
		//Lets associate our texture with this image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1Data);
		//note: above tells openGL how to take someting from ram and store it in vram against our textureID

		//generate mipmaps for this texture
		glGenerateMipmap(GL_TEXTURE_2D);
		//generates a bunch of smaller versions of the texture to be used at great distances so save on processing
	}
	else
	{
		cout << "Image load failed!" << endl;
	}

	//cleanup image memory
	stbi_image_free(image1Data);
	// -----------------------------------------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		processInputs(window);

		//set openGL clear colour
		glClearColor(0, 0, 1, 1);//r,g,b,a as floats, 0 to 1
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW SHIT
		//-------------------------------------------------
		//TEXTURED RECTANGLE
		shaderProgram1.use();
		//we're going to set our texture against fragment shaders texture 1 uniform
		//get uniforms location id
		int texture1Uniform = glGetUniformLocation(shaderProgram1.ID, "texture1");

		//Make our texture an active texture in slot texture0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1ID);

		//set value at this location
		glUniform1i(texture1Uniform, 0);//1i = 1 int to pass in.
										//the zero we passed in is to say we wish this uniform value to ref active texture0 of 16

										//bind our VAO which has info about our rectangle
		glBindVertexArray(textureRectVAO);

		//draw our textured rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//-------------------------------------------------

		//Input for window
		glfwPollEvents();

		//swap render buffers with this loops rendered scene
		glfwSwapBuffers(window);
	}

	glfwTerminate();
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