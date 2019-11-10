#pragma once
#include "gl.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
	
public:

	ShaderProgram();
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	~ShaderProgram();

	
	////utility Uniform functions
	void setBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), (int)value); };
	void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), value); };
	void setFloat(const std::string &name, float value) const { glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), value); };
	void setMat4(const std::string name, glm::mat4 value) const {
		unsigned int transformLoc = glGetUniformLocation(_shaderProgram, name.c_str());
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
	};
	

	unsigned int getProgram() { return _shaderProgram; }

private:

	unsigned int _shaderProgram;
	
	
};



