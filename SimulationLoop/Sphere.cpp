
#include "Sphere.h"
#include <gl\gl.h>  
#include <utility>

#define _USE_MATH_DEFINES

int Sphere::countID = 0;

Sphere::Sphere(std::vector<Vertex> vertices, std::vector<unsigned int> indices, State state) : Mesh(std::move(vertices),
                                                                                       std::move(indices)), m_mass(0.2f), m_radius(0.5), m_state(
	                                                                                               std::move(state))
{
	m_objectID = countID;
	++countID;

	
}

Sphere::~Sphere(void)
= default;

void Sphere::SetPos(const glm::vec3& pos)
{
	m_state.position = pos;
}

void Sphere::SetNewPos(const glm::vec3& pos)
{
	m_newState.position = pos;
}

void Sphere::SetVel(const glm::vec3& vel)
{
	m_state.velocity = vel;
}

void Sphere::SetNewVel(const glm::vec3& vel)
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

//void Sphere::CollisionWithSphere(Sphere* others_sphere, std::unique_ptr<ContactManifold> contactManifold)
//{
//
//	const glm::vec3 pos1 = this->GetNewPos();
//	const glm::vec3 pos2 = others_sphere->GetNewPos();
//	const float dist = pos1.distance(pos2) - (this->GetRadius() + others_sphere->GetRadius());
//	if (dist < 0.0f)
//	{
//		ManifoldPoint mp;
//		mp.contactID1 = this;
//		mp.contactID2 = others_sphere;
//		mp.contactNormal = (pos2 - pos1).normalise();
//		contactManifold->Add(mp);
//	}
//
//}

void Sphere::CollisionWithSphere(Sphere* others_sphere, ContactManifold *contactManifold)
{

	const glm::vec3 pos1 = this->GetNewPos();
	const glm::vec3 pos2 = others_sphere->GetNewPos();
	const float dist = glm::distance(pos1, pos2) - (this->GetRadius() + others_sphere->GetRadius());
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;
		mp.contactID2 = others_sphere;
		mp.contactNormal =  glm::normalize (pos2 - pos1);
		contactManifold->Add(mp);
	}

}

void Sphere::CollisionWithPlane(Sphere* sphere, float time, ContactManifold* contactManifold) {

	const auto c = glm::vec3(-5, -8, 5);
	const auto a = glm::vec3(-5, -8, -5);
	const auto b = glm::vec3(5, -8, -5);
	//const auto normal = glm::normalize(glm::cross(b - a, c - a));
	const auto normal = glm::vec3(0.0f, -1.0f, 0.0f);
	const auto d = glm::dot(normal, a);
	
	const auto dist = glm::dot(normal, sphere->GetPos())-d;

	if (glm::abs(dist) <= sphere->GetRadius()) {
		sphere->SetNewPos(sphere->GetPos());
		sphere->SetNewVel(sphere->GetVel());
	}
	else {

		const auto denom = glm::dot(normal, sphere->GetVel());
		if (denom * dist < 0) {

			const auto r = dist > 0.0f ? sphere->GetRadius() : -sphere->GetRadius();
			const auto timeOfImpact = (r - dist) / denom;
			if(timeOfImpact <= time) {
				
				const auto pointOfImpact = sphere->GetPos() + timeOfImpact * sphere->GetVel() - r * normal ;
				ManifoldPoint mp;
				mp.contactID1 = this;
				mp.contactID2 = nullptr;
				mp.plane.pointOfImpact = pointOfImpact;
				mp.plane.time = time;
				mp.contactNormal = normal;
				contactManifold->Add(mp);
				
			}
			
		}
		
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

	const glm::vec3 colNormal = point.contactNormal;
	
	point.contactID1->SetNewPos(point.plane.pointOfImpact - point.contactID1->GetRadius() * point.contactNormal);
	point.contactID1->SetNewVel(-0.8f*colNormal* glm::dot(colNormal, point.contactID1->GetVel()) );
	
	if(point.contactID2) {
		 
		point.contactID2->ResetPos();
		point.contactID2->SetNewVel(-0.8f*colNormal* glm::dot(colNormal, point.contactID2->GetVel()));
		
	}
	
	
	
}

float Sphere::GetMass() const
{
	return m_mass;
}

glm::vec3 Sphere::GetPos() const
{
	return m_state.position;
}

glm::vec3 Sphere::GetNewPos() const
{
	return m_newState.position;
}

glm::vec3 Sphere::GetVel() const
{
	return m_state.velocity;
}

glm::vec3 Sphere::GetNewVel() const
{
	return m_newState.velocity;
}

float Sphere::GetRadius() const
{
	return m_radius;
}


glm::vec3 Sphere::force(const State& state, float t) const {

	const glm::vec3 gravity(0.0f, -9.81f * 3, 0.0f);
	const float k = 10; //k*pos thrust
	const float b = 1.0f;  //drag coefficient
	return  (-k * state.position - m_mass * state.velocity) / t + gravity;
	
}

void Sphere::integrate(State& state, float t, float dt) {

	const auto k1 = Evaluate(state, t, 0.0f, Derivative());
	const auto k2 = Evaluate(state, t, dt * 0.5f, k1);
	const auto k3 = Evaluate(state, t, dt * 0.5f, k2);
	const auto k4 = Evaluate(state, t, dt, k3);

	const glm::vec3 dxdt = 1.0f / 6.0f * (k1.dx + 2.0f * (k2.dx + k3.dx) + k4.dx);

	const glm::vec3 dvdt = 1.0f / 6.0f * (k1.dv + 2.0f * (k2.dv + k3.dv) + k4.dv);

	m_newState.position = state.position + dxdt * dt;
	m_newState.velocity = state.velocity + dvdt * dt;

	
	//if (m_newState.position.y < -10.0f + m_radius)
	//{
	//	m_newState.position = glm::vec3(state.position.x, -10.0f + m_radius, state.position.z);
	//	m_newState.velocity = glm::vec3(state.velocity.x, -state.velocity.y*0.8f, state.velocity.z);
	//	
	//}
	
}


Derivative Sphere::Evaluate(const State &initial, const float t, const float dt, const Derivative &d) const {

	State state;
	state.position = initial.position + d.dx * dt;
	state.velocity = initial.velocity + d.dv * dt;

	Derivative output;

	output.dx = state.velocity;
	output.dv = force(state, t + dt);



	return output;
}



