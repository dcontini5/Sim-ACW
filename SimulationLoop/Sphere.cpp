
#include "Sphere.h"
#include <gl\gl.h>  
#include <utility>

#define _USE_MATH_DEFINES

int Sphere::countID = 0;

Sphere::Sphere(std::vector<Vertex> vertices, std::vector<unsigned int> indices, State state) : Mesh(std::move(vertices),
                                                                                       std::move(indices)), m_mass(1), m_radius(0.5), m_state(
	                                                                                               std::move(state))
{
	m_objectID = countID;
	++countID;

	
}

Sphere::~Sphere(void)
= default;

void Sphere::SetPos(const float x, const float y)
{
	m_state.position.Set(x, y);
}

void Sphere::SetNewPos(const Vector2f& pos)
{
	m_newState.position = pos;
}

void Sphere::SetVel(const float x, const float y)
{
	m_state.velocity.Set(x, y);
}

void Sphere::SetNewVel(const Vector2f& vel)
{
	m_newState.velocity = vel;
}

void Sphere::SetMass(const float mass)
{
	m_mass = mass;
}




void Sphere::CalculatePhysics(const float t, const float dt)
{

	// Integration /w Runge-Kutta
	integrate(m_state, t, dt);

}


void Sphere::CollisionWithSphere(Sphere* others_sphere, ContactManifold *contactManifold)
{

	const Vector2f pos1 = this->GetNewPos();
	const Vector2f pos2 = others_sphere->GetNewPos();
	const float dist = pos1.distance(pos2) - (this->GetRadius() + others_sphere->GetRadius());
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;
		mp.contactID2 = others_sphere;
		mp.contactNormal = (pos2 - pos1).normalise();
		contactManifold->Add(mp);
	}

}

void Sphere::ResetPos()
{
	m_newState.position = m_state.position;
}

void Sphere::Update()
{
	m_state.velocity = m_newState.velocity;
	m_state.position = m_newState.position;
}

void Sphere::CollisionResponseWithSphere(ManifoldPoint &point)
{
	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model responses between Spheres     ********
	// ****************************************************************
	// ******** Replace with better response code for Spheres  ********
	// ****************************************************************

	const Vector2f colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(-0.8f*colNormal*colNormal.dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(-0.8f*colNormal*colNormal.dot(point.contactID2->GetVel()));
	
}

float Sphere::GetMass() const
{
	return m_mass;
}

Vector2f Sphere::GetPos() const
{
	return m_state.position;
}

Vector2f Sphere::GetNewPos() const
{
	return m_newState.position;
}

Vector2f Sphere::GetVel() const
{
	return m_state.velocity;
}

Vector2f Sphere::GetNewVel() const
{
	return m_newState.velocity;
}

float Sphere::GetRadius() const
{
	return m_radius;
}


Vector2f Sphere::force(const State& state, float t) {

	const Vector2f force(0.0f, -9.81f * 3);
	const float k = 5; //k*pos thrust
	const float b = 2.0f;  //drag coefficient
	return  ((-k * state.position - b * state.velocity )  / t) + force;
	
}

void Sphere::integrate(State& state, float t, float dt) {

	const auto k1 = Evaluate(state, t, 0.0f, Derivative());
	const auto k2 = Evaluate(state, t, dt * 0.5f, k1);
	const auto k3 = Evaluate(state, t, dt * 0.5f, k2);
	const auto k4 = Evaluate(state, t, dt, k3);

	const Vector2f dxdt = 1.0f / 6.0f * (k1.dx + 2.0f * (k2.dx + k3.dx) + k4.dx);

	const Vector2f dvdt = 1.0f / 6.0f * (k1.dv + 2.0f * (k2.dv + k3.dv) + k4.dv);

	m_newState.position = state.position + dxdt * dt;
	m_newState.velocity = state.velocity + dvdt * dt;

	
	if (m_newState.position.GetY() < -10.0f + m_radius)
	{
		m_newState.position.Set(state.position.GetX(), -10.0f + m_radius);
		m_newState.velocity.Set(state.velocity.GetX(), -state.velocity.GetY()*0.8f);
		
	}
	
}


Derivative Sphere::Evaluate(const State &initial, float t, float dt, const Derivative &d) {

	State state;
	state.position = initial.position + d.dx * dt;
	state.velocity = initial.velocity + d.dv * dt;

	Derivative output;

	output.dx = state.velocity;
	output.dv = force(state, t + dt);



	return output;
}

void Sphere::stepSimulation(float dt){



}

