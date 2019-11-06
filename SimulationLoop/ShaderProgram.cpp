#include "gl.h"
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(){

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	//check for compile errors
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLILATION_FAILED\n" << infolog << std::endl;


	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	//chech fragment shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {

		glGetProgramInfoLog(fragmentShader, 512, nullptr, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLILATION_FAILED\n" << infolog << std::endl;


	}

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);

	//check for linking errors
	glGetProgramiv(_shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {

		glGetProgramInfoLog(_shaderProgram, 512, nullptr, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPLILATION_FAILED\n" << infolog << std::endl;


	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
}


ShaderProgram::~ShaderProgram()
{
}


