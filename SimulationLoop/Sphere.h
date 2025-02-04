#pragma once

#include "gl.h"
#include <GLFW/glfw3.h>
#include "ContactManifold.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"


struct State {

	glm::vec3 position; 
	glm::vec3 velocity; 
	
};


struct PlaneInfo {

	//glm::vec3 botL; //bottom left corner
	//glm::vec3 botR;	//bottom right corner
	glm::vec3 topL;	//top left corner
	//glm::vec3 topR;	//top right corner
	glm::vec3 normal;
	float d;
	State state;
	State new_state;
	float direction;
	
};

struct Derivative {

	glm::vec3 dx; //dx/dt = position
	glm::vec3 dv; //dv/dt = velocity
	
	
};



class Sphere : public Mesh
{
public:
	Sphere(std::vector<Vertex> vertices, std::vector<unsigned int> indices, State state, float radius);
	~Sphere(void);

	void CalculatePhysics(float dt, float t);
	void CollisionWithSphere(Sphere* otherSphere, float time, ContactManifold *contactManifold);
	//void CollisionWithSphere(Sphere* others_sphere, std::unique_ptr<ContactManifold> contactManifold);
	void CollisionWithPlane(Sphere* sphere, float time, ContactManifold* contactManifold);
	void CollisionWithTopPlane(PlaneInfo plane, float time, ContactManifold* contactManifold);
	void CollisionWithPlane(PlaneInfo plane, float time, ContactManifold* contactManifold);
	void CollisionWithBowl(Sphere* sphere, float time, ContactManifold* contactManifold);
	void Update();
	static void CollisionResponseWithSphere(ManifoldPoint &point, float e);
	void SetPos(const glm::vec3& pos);
	void SetVel(const glm::vec3& vel);
	void SetNewPos(const glm::vec3& pos);
	void SetNewVel(const glm::vec3& vel);
	void SetMass(float mass);
	
	glm::vec3 GetPos() const;
	glm::vec3 GetNewPos() const;
	glm::vec3 GetVel() const;
	glm::vec3 GetNewVel() const;
	float GetMass() const;
	void ResetPos();
	float GetRadius() const;

	glm::vec3 force(const State& state, float t) const;
	void integrate(State &state, float t, float dt);
	Derivative Evaluate(const State& initial, float t, float dt, const Derivative& d) const;
	static void stepSimulation(float dt);


	
	
private:
	
	float m_mass;
	float m_radius;
	State m_state;
	State m_newState;
	int m_objectID;
	GLuint m_texture;
	
	float coeficentOfElasicity;
	
	static int countID;
};

