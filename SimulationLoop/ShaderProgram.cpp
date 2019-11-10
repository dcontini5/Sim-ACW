#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath) {

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		fShaderFile.close();
		vShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}catch (std::ifstream::failure e) {

		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		
	}


	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
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
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
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


ShaderProgram::~ShaderProgram(){
}


