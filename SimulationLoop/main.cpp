#include "gl.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#include "Game.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

Game *game;



int main(int argc, char *argv[], char *envp[]){

	
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Window Creation
	GLFWwindow * window = glfwCreateWindow(800, 600, "Simulation Loop", nullptr, nullptr);
	if(!window) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
		
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	game = new Game();

	



	//GAME LOOP
	
	while (!glfwWindowShouldClose(window)) {

		
		processInput(window);

		game->Update();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
/*
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
*/
	glfwTerminate();
	return 0;
	
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
