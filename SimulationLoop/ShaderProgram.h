#pragma once
#include <string>


class ShaderProgram
{
	
public:

	ShaderProgram();
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	~ShaderProgram();

	
	////utility Uniform functions
	//void setBool(const std::string &name, bool value) const;
	//void setInt(const std::string &name, int value) const;
	//void setFloat(const std::string &name, float value) const;

	unsigned int getProgram() { return _shaderProgram; }

private:

	unsigned int _shaderProgram;
	
	
};



