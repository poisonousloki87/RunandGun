#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <sstream>//string stream

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "GameLoop.h"

using namespace std;

bool wireFrame = false;
int selectedColour = 1;
GameLoop gameLoop;

Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;

//window resize call back function prototype
void windowResizeCallBack(GLFWwindow* window, int width, int height);

//user inputs
void processInputs(GLFWwindow* window);

//Frames Per Second prototype
void showFPS(GLFWwindow* window); 

//mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//scroll wheel callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void main()
{
	glfwInit();
	//tell glfw that we want to work with openGL 3.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //the first 3 of 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //the .3 of 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile

	//build window
	GLFWwindow *window = glfwCreateWindow(800, 600, "My first OpenGL thing", NULL, NULL);
	//if it fails
	if (window == NULL){
		//try report error
		cout << "failed to create window" << endl;
		glfwTerminate(); //cleanup glfw stuff
		system("pause");
		return;
	}

	//make this window the current one
	glfwMakeContextCurrent(window);

	//initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		//if this fails, then
		cout << "GLAD failed to initialise" << endl;
		glfwTerminate(); //cleanup glfw stuff
		system("pause");
		return;
	}

	//set up openGL viewport x,y,w,h
	glViewport(0, 0, 800, 600);//you dont have to use the full window for openGL but we will
	
	//load image in, flip them
	stbi_set_flip_vertically_on_load(true);

	//add window resize callback, params: window to check events on, function to call
		//when it resizes
	glfwSetFramebufferSizeCallback(window, windowResizeCallBack);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);



	//hide cursor and lock mouse within window area
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader shaderProgram3("vertexShader4.txt", "fragmentShader4.txt");
	Shader shaderProgram4("vertexShader5.txt", "fragmentShader4.txt");

	//the vertices order, treating each xyz as an index
	unsigned int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3 // second triangle
	};
	
	float colourRectVertices[] = {
		// positions // colors // texture coords
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left 
	};

	float startButtonVertices[] = {
		// positions // colors // texture coords
		0.5f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left  
	};
	
	//BACKGROUND-----------------------------------------------------------
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(colourRectVertices), colourRectVertices, GL_STATIC_DRAW);

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

	//LOAD UP IMAGE FILE
	int width, height, numberChannels; //as we load an image, we'll get values from it to fill these in
	unsigned char *image1Data = stbi_load("Assets/Background.png", &width, &height, &numberChannels, 0);
	//if it loaded
	if (image1Data){
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

	//----------------------------------------------------------------------------------------
	

	//BUTTON1
	//----------------------------------------------------------------------------------------
	//CREATE VBO TO STORE VERTICES
	unsigned int button1VBO;
	glGenBuffers(1, &button1VBO);
	//CREATE EBO TO STORE CONNECTION INDEXES 
	unsigned int button1EBO;
	glGenBuffers(1, &button1EBO);
	//CREATE VAO TO STORE OPERATIONS ON VBO
	unsigned int button1VAO;
	glGenVertexArrays(1, &button1VAO);
	//to work with this VAO bind it to make it the current one
	glBindVertexArray(button1VAO);
	//bind the buffer object to this vao
	glBindBuffer(GL_ARRAY_BUFFER, button1VBO);
	//give data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(startButtonVertices), startButtonVertices, GL_STATIC_DRAW);

	//bind elemental buffer object to this vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button1EBO);
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
	unsigned int texture2ID;
	glGenTextures(1, &texture2ID);
	//we bind the texture to make it the one we're working on
	glBindTexture(GL_TEXTURE_2D, texture2ID);
	//set wrapping options(repeat texture if texture coordinates dont fully cover polygons)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//wrap on the s(x) axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//wraps on the t(y) axis
																 //set filtering options
																 //Suggestion use nearest neighbour for pixel art, use bilinear for pretty much everything else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR(bilinear) or GL_NEAREST for shrinking
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//for stretching

																	 //LOAD UP IMAGE FILE
	int width1, height1, numberChannels1; //as we load an image, we'll get values from it to fill these in
	unsigned char *image2Data = stbi_load("Assets/StartGameButton.png", &width1, &height1, &numberChannels1, 0);
	//if it loaded
	if (image2Data) {
		cout << "Success! Image is " << width1 << " by " << height1 << "pixels" << endl;
		//Lets associate our texture with this image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image2Data);
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
	stbi_image_free(image2Data);
	//----------------------------------------------------------------------------------------

	//BUTTON2
	//----------------------------------------------------------------------------------------
	//CREATE VBO TO STORE VERTICES
	unsigned int button2VBO;
	glGenBuffers(1, &button2VBO);
	//CREATE EBO TO STORE CONNECTION INDEXES 
	unsigned int button2EBO;
	glGenBuffers(1, &button2EBO);
	//CREATE VAO TO STORE OPERATIONS ON VBO
	unsigned int button2VAO;
	glGenVertexArrays(1, &button2VAO);
	//to work with this VAO bind it to make it the current one
	glBindVertexArray(button2VAO);
	//bind the buffer object to this vao
	glBindBuffer(GL_ARRAY_BUFFER, button2VBO);
	//give data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(startButtonVertices), startButtonVertices, GL_STATIC_DRAW);

	//bind elemental buffer object to this vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button2EBO);
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
	unsigned int texture3ID;
	glGenTextures(1, &texture3ID);
	//we bind the texture to make it the one we're working on
	glBindTexture(GL_TEXTURE_2D, texture3ID);
	//set wrapping options(repeat texture if texture coordinates dont fully cover polygons)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//wrap on the s(x) axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//wraps on the t(y) axis
																 //set filtering options
																 //Suggestion use nearest neighbour for pixel art, use bilinear for pretty much everything else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR(bilinear) or GL_NEAREST for shrinking
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//for stretching

																	 //LOAD UP IMAGE FILE
	int width2, height2, numberChannels2; //as we load an image, we'll get values from it to fill these in
	unsigned char *image3Data = stbi_load("Assets/QuitButton.png", &width2, &height2, &numberChannels2, 0);
	//if it loaded
	if (image2Data) {
		cout << "Success! Image is " << width2 << " by " << height2 << "pixels" << endl;
		//Lets associate our texture with this image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image3Data);
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
	stbi_image_free(image3Data);
	//----------------------------------------------------------------------------------------


	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		//user inputs
		processInputs(window);

		shaderProgram3.use();
		//BACKGROUNG-----------------------------------------
		//Make our texture an active texture in slot texture0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1ID);

		//bind our VAO which has info about our rectangle
		glBindVertexArray(textureRectVAO);
		
		//draw our textured rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//--------------------------------------------------

		shaderProgram4.use();
		//BUTTON1-------------------------------------------	
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));

		glm::mat4 view = glm::mat4(1.0f);
		//view sort of represents the camera, we translate the scene in reverse to what we think
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//lets apply perspective to our scene now using a projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		//								field of veiw			screenWidth/screenHeight, near distance, to far distance
		projection = glm::perspective(glm::radians(54.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		//pass these matrixes to the shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2ID);

		//bind our VAO which has info about our rectangle
		glBindVertexArray(button1VAO);

		//draw our textured rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//--------------------------------------------------
		//BUTTON2-------------------------------------------	
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));

		view = glm::mat4(1.0f);
		//view sort of represents the camera, we translate the scene in reverse to what we think
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//lets apply perspective to our scene now using a projection matrix
		projection = glm::mat4(1.0f);
		//								field of veiw			screenWidth/screenHeight, near distance, to far distance
		projection = glm::perspective(glm::radians(54.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		//pass these matrixes to the shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram4.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3ID);

		//bind our VAO which has info about our rectangle
		glBindVertexArray(button2VAO);

		//draw our textured rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//--------------------------------------------------


		//IF BUTTON CLICKED THEN RUN THE GAME WORLD
		// 
		//

		//Input for window
		glfwPollEvents();

		//swap render buffers with this loops rendered scene
		glfwSwapBuffers(window);

		showFPS(window);
	}

	//optional: de-allocate all resources
	//glDeleteBuffers(2, VBOs); //example of deleting 2 VBO ids from the VBOs array


	glfwTerminate();
}

//window resize call back function prototype
void windowResizeCallBack(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

//user inputs
void processInputs(GLFWwindow* window){
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		gameLoop.RunGameLoop(window, camera);
	}
}

void showFPS(GLFWwindow* window){
	//static function variables are declared 
	//once per project run, so these 2 lines of 
	//code run once and then the variables persist
	//until the end of the program
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();
		//getTime returns seconds since startup

	elapsedSeconds = currentSeconds - previousSeconds;
	if (elapsedSeconds > 0.25){
		previousSeconds = currentSeconds;
		double fps = frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		stringstream ss;
		ss.precision(3);//3 decimal places
		ss << fixed << "RunAndGun FPS: " << fps << " Frame Time: " << msPerFrame << "(ms)";

		glfwSetWindowTitle(window, ss.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}

//mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		//helps not make a massive camera rotation jump when mouse first touches the screen
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

//scroll wheel callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->ProcessMouseScroll(yoffset);
}

