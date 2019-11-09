#pragma once

#include "gl.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	void Render();
	void Render( ShaderProgram* shaderProgram, const glm::vec3 position) const;


private:

	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	unsigned int VAO;


};

