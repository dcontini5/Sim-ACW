#include "Mesh.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices){

	this->_vertices = vertices;
	this->_indices = indices;

	unsigned int VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//bind the vertex array object first, then bind the set vertex buffers, and then configure vertex attributes.

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , (void*)0);
	
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


}




Mesh::~Mesh(){
}

void Mesh::Render() {
		
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	

}

void Mesh::Render(ShaderProgram* shaderProgram, const glm::vec3 position) const {

	glm::mat4 trans = glm::mat4(1);

	trans = glm::translate(trans, position);
	shaderProgram->setMat4("transform", trans);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::Render(ShaderProgram* shaderProgram, const glm::mat4 position) const {

	glm::mat4 trans = glm::mat4(1);

	trans *= position;
	shaderProgram->setMat4("transform", trans);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

}
