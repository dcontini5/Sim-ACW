#pragma once

#include <Windows.h>
#include "Sphere.h"
#include "ContactManifold.h"
#include "ShaderProgram.h"



const std::vector<Vertex> vertices = {
	{ glm::vec3(0.5f,  0.5f, 0.0f) } ,	 // top right
	{ glm::vec3(0.5f, -0.5f, 0.0f) } ,	 // bottom right
	{ glm::vec3(-0.5f, -0.5f, 0.0f) } ,	 // bottom left
	{ glm::vec3(-0.5f,  0.5f, 0.0f) } ,	 // top left 
};

const std::vector<unsigned int> indices = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

struct Geometry {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	

};

class Game
{
public:
	Game();
	Game(HDC hdc);
	~Game(void);

	void Update();

	glm::vec3 GetCameraPos() { return _cameraPos; };
	glm::vec3 GetCameraFront() { return _cameraFront; };
	glm::vec3 GetCameraUp() { return _cameraUp; };
	void SetCameraPos(glm::vec3 newPos) { _cameraPos = newPos; };
	void UpdateView() { _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp); };
	//void SetCameraFront(glm::vec3 newFront);
	//void SetCameraUp(glm::vec3 newUp);


private:
	void SimulationLoop();
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();
	void Render();
	void CreateSphereGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	void CreateCilinderGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	Geometry CreateBoxGeometry();
	Geometry CreateTrayGeometry();
	Geometry CreateTopTrayGeometry();

	
	

private:

	ShaderProgram *m_shader_program;
	float m_dt;
	int	  m_fps;
	float m_previousTime;
	Sphere *m_sphere1;
	Sphere *m_sphere2;
	Sphere *m_sphere3;
	ContactManifold *m_manifold;
	LARGE_INTEGER start, end, frequency;
	Geometry _sphereGeometry;
	Geometry _cilinderGeometry;
	Mesh* _box;
	Mesh* _bottomTray ;
	Mesh* _topTray ;
	Mesh* _cilinder ;
	Mesh* _bowl ;
	glm::mat4 _proj, _model, _view;
	glm::vec3 _cameraPos, _cameraFront, _cameraUp;

};

