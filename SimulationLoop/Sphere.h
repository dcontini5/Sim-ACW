#pragma once

#include "ContactManifold.h"
#include "Vector2f.h"
#include <Windows.h>
#include <gl\gl.h>  

struct Derivative {

	Vector2f dx; //dx/dt = position
	Vector2f dv; //dv/dt = velocity
	
	
};

struct State {

	Vector2f position; 
	Vector2f velocity; 
	
};

class Sphere
{
public:
	Sphere(void);
	~Sphere(void);

	void CalculatePhysics(float dt);
	void CalculatePhysics(float dt, float t);
	void CollisionWithSphere(Sphere* sphere, ContactManifold *contactManifold);
	void Update();
	void CollisionResponseWithSphere(ManifoldPoint &point);
	void SetPos(float x, float y);
	void SetVel(float x, float y);
	void SetNewPos(Vector2f pos);
	void SetNewVel(Vector2f vel);
	void SetMass(float mass);
	
	Vector2f GetPos() const;
	Vector2f GetNewPos() const;
	Vector2f GetVel() const;
	Vector2f GetNewVel() const;
	float GetMass() const;
	void ResetPos();
	float GetRadius() const;

	void Render() const;
	static Vector2f force(const State& state, float t);
//	static Vector2f acceleration(const State& state, float t);
	void integrate(State &state, float t, float dt);
	static Derivative Evaluate(const State& initial, float t, float dt, const Derivative& d);
	void Recalculate();

private:
	float m_mass;
	float m_radius;
	State m_state;
	State m_newState;
	int m_objectID;
	GLuint m_texture;

	static int countID;
};

