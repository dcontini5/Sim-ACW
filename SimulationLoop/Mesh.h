#pragma once

#include "gl.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

//#define DEBUG

struct Vertex {

	glm::vec3 position;
	//glm::vec4 color;
	//glm::vec2 texCoord;

};


class Mesh
{



public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	void Render( ShaderProgram* shaderProgram, const glm::vec3 position) const;
	void Render( ShaderProgram* shaderProgram, const glm::mat4 position) const;


private:

	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	unsigned int VAO{};


};

