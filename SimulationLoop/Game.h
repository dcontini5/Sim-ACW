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
	void UpdateModel(float rot) { _model = glm::rotate(_model, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }

	void AddBall();
	void RemoveTopTray() {
		_moveTopTray = true;
		topTray.direction = 1.0f;
	};
	void InsertTopTray() {
		_moveTopTray = true;
		topTray.direction = -1.0f;
	};
	void RemoveBottomTray() { _moveBottomTray = true; _planeList[4].direction = 1.0f; };
	void InsertBottomTray() { _moveBottomTray = true; _planeList[4].direction = -1.0f; };
	void TogglePause() { if (m_previous_time_ - _pauseCD > 1.0f) { _pause = !_pause; _pauseCD = m_previous_time_; } };
	void CalculateRotorSpeed() { _rotorSpeed += glm::pi<float>() / 4.0f * _speedMult * m_dt; };
	void SetPropellerSpeed(float speedIncr) { _speedMult += speedIncr; };

	
private:
	void SimulationLoop();
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse() const;
	void UpdateObjectPhysics();
	void Render();
	void CreateSphereGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	void CreateCylinderGeometry(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);
	Geometry CreateBoxGeometry();
	Geometry CreateTrayGeometry();
	Geometry CreateTopTrayGeometry();
	bool moveTray(PlaneInfo& plane, float dt);
	void UpdateTrays(State& state, State& newState);


private:

	ShaderProgram *m_shader_program;
	float m_dt;
	int	  m_fps;
	float m_previous_time_;
	std::vector<Sphere*> _sphereList;
	std::vector<PlaneInfo> _planeList;
	PlaneInfo topTray;
	LARGE_INTEGER start, end, frequency;
	Geometry _sphereGeometry;
	Geometry _cylinderGeometry;
	Sphere* lastsphere;
	bool _pause;
	float _pauseCD;
	Mesh* _box;
	Mesh* _bottomTray ;
	Mesh* _topTray ;
	Mesh* _cylinder ;
	Mesh* _bowl ;
	ContactManifold* m_manifold;
	bool _moveTopTray;
	float  _topTrayDirection;
	bool _moveBottomTray;
	float  _bottomTrayDirection;
	glm::mat4 _proj, _model, _view;
	glm::vec3 _cameraPos, _cameraFront, _cameraUp;
	float _rotorSpeed;
	float _speedMult;
	
};

