#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragementPath) {
	string vertexCode;
	string fragementCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragementPath);
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragementCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "Shader file did not successfully read" << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragementCode.c_str();

	unsigned int vertexShaderID, fragmentShaderID;
	int success;

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID, "VERTEX");

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &vShaderCode, NULL);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShaderID);
	glAttachShader(ID, fragmentShaderID);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
void Shader::use() {
	glUseProgram(ID);
}
void Shader::checkCompileErrors(unsigned int shaderID, string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
			cout << "Shader Compile Error: " << infoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
			cout << "Program Linking Error: " << infoLog << endl;
		}
	}
}