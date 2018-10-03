#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <sstream>//string stream

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"

using namespace std;

class GameLoop
{
private:
	bool gameRunning;

	//Time management stuff
	float deltaTime; //time between current frame and last frame
	float lastFrame; //time of last frame
public:
	GameLoop();
	~GameLoop();

	void RunGameLoop(GLFWwindow* window, Camera* camera);
	void ProcessGameInputs(GLFWwindow* window, Camera* camera);
};

