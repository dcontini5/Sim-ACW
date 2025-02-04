#include "gl.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#include "Game.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

Game *game;



int main(){

	
	
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


	glEnable(GL_DEPTH_TEST);



	//GAME LOOP
	
	while (!glfwWindowShouldClose(window)) {

		
		processInput(window);

		game->Update();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
	
}

void processInput(GLFWwindow *window)
{
	auto cameraSpeed = 0.1f;
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		game->AddBall();
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		game->InsertTopTray();
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		game->RemoveTopTray();
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		game->InsertBottomTray();
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		game->RemoveBottomTray();
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		//delete game;
		delete game;
		game = new Game(); //todo solve memory leak
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		game->TogglePause();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() + cameraSpeed * game->GetCameraFront());
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() - cameraSpeed * game->GetCameraFront());
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		game->UpdateModel(2.0f * -cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		game->UpdateModel(2.0f * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() + cameraSpeed * game->GetCameraUp());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() - cameraSpeed * game->GetCameraUp());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() - glm::normalize(glm::cross(game->GetCameraFront(), game->GetCameraUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		game->SetCameraPos(game->GetCameraPos() + glm::normalize(glm::cross(game->GetCameraFront(), game->GetCameraUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		game->SetPropellerSpeed(-cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		game->SetPropellerSpeed(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		game->setCoE(0.01f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		game->setCoE(-0.01f);
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		game->setRadius(0.1f);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		game->setRadius(-0.1f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		game->setFric(0.01f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		game->setFric(-0.01f);

	game->UpdateView();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	
}
