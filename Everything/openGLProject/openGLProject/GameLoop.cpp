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

	//NEW SHADERS
	Shader lightShader("modelShader.vs", "modelShader.fs");
	Shader lampShader("shader6.vs", "lampShader.fs");

	// load models
	Model ourModel("Assets/PacManMap.obj");
	//Model pacManModel("Assets/.obj");
	//Model ghostModel("Assets/.obj");

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
		//cout << "SHIT IS HAPPENING!" << endl;
		glm::mat4 view = camera->GetViewMatrix();//glm::mat4(1.0f);									
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		lightShader.use();
		lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightShader.setVec3("lightColor", lightColour);
		lightShader.setVec3("lightPos", lightPos);
		lightShader.setVec3("viewPos", camera->Position);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)); // ROTATE to match game world
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)); // ROTATE to match game world
		model = glm::translate(model, glm::vec3(0.35f, 1.0f, -0.4f)); // MOVE to infront of the camer
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// SCALE so it fits the camera screen
		lightShader.setMat4("model", model);
		ourModel.Draw(lightShader);

		//LAMP
		lampShader.use();
		lampShader.setVec3("lightColour", lightColour);

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
	//close the game
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		gameRunning = false;
	}
	
	//camera walking
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
}