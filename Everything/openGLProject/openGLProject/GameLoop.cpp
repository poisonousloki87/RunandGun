#include "GameLoop.h"

GameLoop::GameLoop()
{
	gameRunning = true;
	//Time management stuff
	deltaTime = 0.0f; //time between current frame and last frame
	lastFrame = 0.0f; //time of last frame
}

GameLoop::~GameLoop()
{
}

void GameLoop::RunGameLoop(GLFWwindow* window, Camera* camera)
{
	//lighting globals
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
	//glm::vec3 mapPos(0, 0, 0);
	//glm::vec3 playerPos(1,1,1);
	//glm::vec3 enemyPos(2,2,2);

	unsigned int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3 // second triangle
	};

	/*CUBE VERT STUFF
	float textureCubVertices[] = {
		//x		y		z	texS   texT  normY normZ  normZ
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};
	*/

	/*OLD SHADERS
	Shader lightShader("lightCubeVertexShader.txt", "lightFragmentShader.txt");
	Shader lampShader("cubeVertexShader.txt", "lampFragmentShader.txt");
	*/
	//NEW SHADERS
	Shader lightShader("modelShader.vs", "modelShader.fs");
	Shader lampShader("shader6.vs", "lampShader.fs");

	//CUBE FOR LIGHT TESTING
	/*
	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);

	//3.Vertex Array Object? tries to describe the data in the VBO and relay it to the first shader
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCubVertices), textureCubVertices, GL_STATIC_DRAW);

	//xyz to location = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture coordinates to location = 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//normal XYZ to location = 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/


	//Game Loop
	while (gameRunning)
	{
		glClearColor(0, 0, 1, 1);//r,g,b,a as floats, 0 to 1
		//clear screen AND clear Z depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//update our time management stuff
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//DRAW OBJECTS
		cout << "SHIT IS HAPPENING!" << endl;
		glm::mat4 view = camera->GetViewMatrix();//glm::mat4(1.0f);									
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		//LAMP
		lampShader.use();
		lampShader.setVec3("lightColour", lightColour);
		/* DRAW FOR THE CUBE
		//bind our cube VAO so we can draw a cube shape
		glBindVertexArray(cubeVAO);
		glm::mat4 lampModel = glm::mat4(1.0f);	//matrix describing where our lamp is in world space
		lampModel = glm::translate(lampModel, lightPos);	//takes an existing matrix and moves(translates) it to the new position
		lampModel = glm::rotate(lampModel, glm::radians(45.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		lampModel = glm::scale(lampModel, glm::vec3(2.0f, 2.0f, 2.0f));
		lampShader.setMat4("model", lampModel);
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		//draw our lamp from the currently bound VBO
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

		//Inputs
		ProcessGameInputs(window, camera);
		//Input for window
		glfwPollEvents();

		//swap render buffers with this loops rendered scene
		glfwSwapBuffers(window);
	}
}

void GameLoop::ProcessGameInputs(GLFWwindow* window, Camera* camera)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		gameRunning = false;
		cout << "SHIT IS CLOSING!" << endl;
	}

	/*
	//camera walking
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	*/
}