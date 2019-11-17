#pragma once

#include <Windows.h>
#include "Sphere.h"
#include "ContactManifold.h"
#include "ShaderProgram.h"

struct Geometry {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	

};

class Game
{
public:
	Game();
	~Game(void) = default;

	void Update();

	glm::vec3 GetCameraPos() const { return _cameraPos; };
	glm::vec3 GetCameraFront() const { return _cameraFront; };
	glm::vec3 GetCameraUp() const { return _cameraUp; };
	void SetCameraPos(glm::vec3 newPos) { _cameraPos = newPos; };
	void UpdateView() { _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp); };
	//void SetCameraFront(glm::vec3 newFront);
	//void SetCameraUp(glm::vec3 newUp);
	void AddBall();

private:
	void SimulationLoop();
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();
	void Render();
	void CreateSphereGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	void CreateCylinderGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	Geometry CreateBoxGeometry();
	Geometry CreateTrayGeometry();
	Geometry CreateTopTrayGeometry();

	
	

private:

	ShaderProgram *m_shader_program;
	float m_dt;
	int	  m_fps;
	float m_previous_time_;
	std::vector<Sphere*> _sphereList;
	std::vector<PlaneInfo> _planeList;
	//Sphere *m_sphere1;
	//Sphere *m_sphere2;
	//Sphere *m_sphere3;
	LARGE_INTEGER start, end, frequency;
	Geometry _sphereGeometry;
	Geometry _cylinderGeometry;
#ifdef DEBUG
	std::unique_ptr<Mesh> _box;
	std::unique_ptr<Mesh> _bottomTray;
	std::unique_ptr<Mesh> _topTray;
	std::unique_ptr<Mesh> _cylinder;
	std::unique_ptr<Mesh> _bowl;
	std::unique_ptr<ContactManifold>  m_manifold;
#else
	Mesh* _box;
	Mesh* _bottomTray ;
	Mesh* _topTray ;
	Mesh* _cylinder ;
	Mesh* _bowl ;
	ContactManifold* m_manifold;
	
#endif
	glm::mat4 _proj, _model, _view;
	glm::vec3 _cameraPos, _cameraFront, _cameraUp;

};

