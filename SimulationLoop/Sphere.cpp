
#include "Sphere.h"
#include <gl\gl.h>  
#include <utility>

#define _USE_MATH_DEFINES

int Sphere::countID = 0;

Sphere::Sphere(std::vector<Vertex> vertices, std::vector<unsigned int> indices, State state) : Mesh(std::move(vertices),
                                                                                       std::move(indices)), m_mass(2.0f), m_radius(0.5), m_state(
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



void Sphere::CollisionWithSphere(Sphere* otherSphere,float time, ContactManifold *contactManifold)
{

	const glm::vec3 pos1 = this->GetNewPos();
	const glm::vec3 pos2 = otherSphere->GetNewPos();
	float dist = glm::distance(pos1, pos2) - (this->GetRadius() + otherSphere->GetRadius());
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;
		mp.contactID2 = otherSphere;
		mp.contactNormal =  glm::normalize ( abs(pos2 - pos1));
		mp.dist = dist/2;
		contactManifold->Add(mp);
	}/*else {

		const auto denom = glm::dot(otherSphere->GetVel(), this->GetVel());
		if (denom * dist < 0) {

			const auto r = dist > 0.0f ? this->GetRadius() : -this->GetRadius();
			const auto timeOfImpact = (r - dist) / denom;
			if (timeOfImpact <= time) {

				const auto pointOfImpact = this->GetPos() + timeOfImpact * this->GetVel() - otherSphere->GetPos() + timeOfImpact * otherSphere->GetVel()  - (this->GetRadius() + others_sphere->GetRadius());
				const auto normalizedPOI = glm::normalize(this->GetPos() + timeOfImpact * this->GetVel() - otherSphere->GetPos() + timeOfImpact * otherSphere->GetVel());
				dist = glm::distance( this->GetPos() + timeOfImpact * this->GetVel(), otherSphere->GetPos() + timeOfImpact * otherSphere->GetVel() ) - (this->GetRadius() + others_sphere->GetRadius());
				
				ManifoldPoint mp;
				mp.contactID1 = this;
				mp.contactID2 = otherSphere;
				mp.pointOfImpact = pointOfImpact;
				mp.timeAfterCollision = time - timeOfImpact;
				mp.contactNormal = normalizedPOI;
				mp.dist = dist/2;
				contactManifold->Add(mp);


			

			}

		}
		
	}*/

}

//void Sphere::CollisionWithSphere(Sphere* otherSphere, float time, ContactManifold* contactManifold) {
//
//	auto dist =	otherSphere->GetNewPos() -	this->GetNewPos();
//	auto relVelocity = otherSphere->GetNewVel() - this->GetNewVel();
//	auto r = otherSphere->GetRadius() - this->GetRadius();
//	float c = glm::dot(dist, dist) - r * r;
//	float s = glm::distance(otherSphere->GetNewPos(), this->GetNewPos()) - (this->GetRadius() + otherSphere->GetRadius());
//	
//	if(s < 0.0f) {
//
//		ManifoldPoint mp;
//		mp.contactID1 = this;
//		mp.contactID2 = otherSphere;
//		mp.contactNormal =  glm::normalize (dist);
//		mp.dist = s/2;
//		contactManifold->Add(mp);
//		
//	}else {
//
//		float a = dot(relVelocity, relVelocity);
//		if (a < glm::epsilon<float>()) return;
//		float b = dot(relVelocity, dist);
//		if (b >= 0.0f) return;
//		float d = b * b - a * c;
//		if (d < 0.0f) return;
//
//		auto t = (-b - sqrt(d)) / a;
//		
//	}
//
//	
//}

void Sphere::CollisionWithPlane(PlaneInfo plane, float time, ContactManifold* contactManifold) {

	auto dist = glm::dot(plane.normal, this->GetPos()) - plane.d;
	
	if (glm::abs(dist) <= this->GetRadius()) {

		
		if (this->GetPos().x >= -5.0f && this->GetPos().x <= 5.0f &&
			this->GetPos().z >= -5.0f && this->GetPos().z <= 5.0f) {
			
			ManifoldPoint mp;
			mp.contactID1 = this;
			mp.contactID2 = nullptr;
			mp.pointOfImpact = this->GetPos() - dist * plane.normal;
			mp.timeAfterCollision = time;
			mp.contactNormal = plane.normal;
			contactManifold->Add(mp);
			
		}/*	else {

			if (glm::distance((this->GetPos() - dist * plane.normal).x, plane.topL.x) - this->GetRadius() <= 0.0f ){
				
				ManifoldPoint mp;
				mp.contactID1 = this;
				mp.contactID2 = nullptr;
				mp.pointOfImpact = this->GetPos() - dist * plane.normal;
				mp.timeAfterCollision = time;
				mp.contactNormal = glm::normalize(this->GetPos() - dist * plane.normal - glm::vec3(plane.topL.x, plane.topL.y, GetPos().z));
				contactManifold->Add(mp);
			}

		}*/
		
	}else {

		const auto denom = glm::dot(plane.normal, this->GetVel());
		if (denom * dist < 0) {

			const auto r = dist > 0.0f ? this->GetRadius() : -this->GetRadius();
			const auto timeOfImpact = (r - dist) / denom;
			if(timeOfImpact <= time) {
				
				const auto pointOfImpact = this->GetPos() + timeOfImpact * this->GetVel() - r * plane.normal ;
				
				if(pointOfImpact.x >= -5.0f && pointOfImpact.x <= 5.0f &&
					pointOfImpact.z >= -5.0f && pointOfImpact.z <= 5.0f) {
				
					
					ManifoldPoint mp;
					mp.contactID1 = this;
					mp.contactID2 = nullptr;
					mp.pointOfImpact = pointOfImpact;
					mp.timeAfterCollision = time - timeOfImpact;
					mp.contactNormal = plane.normal;
					contactManifold->Add(mp);
					
					
				}else {

					dist = glm::distance((pointOfImpact  + r * plane.normal).x, plane.topL.x) - (this->GetRadius());
					if (dist < 0.0f){
						ManifoldPoint mp;
						mp.contactID1 = this; 
						mp.contactID2 = nullptr;
						mp.pointOfImpact = pointOfImpact + 2 * r * plane.normal;
						mp.timeAfterCollision = time - timeOfImpact;
						mp.contactNormal = glm::normalize(pointOfImpact + r * plane.normal  - glm::vec3(plane.topL.x, plane.topL.y, pointOfImpact.z));
						contactManifold->Add(mp);
					}

				}
									
			}
			
		}
		
	}

}

void Sphere::CollisionWithBowl(Sphere* sphere, float time, ContactManifold* contactManifold) {

	
	
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
	
	
	//point.contactID1->SetNewVel(-0.8f*colNormal* glm::dot(colNormal, point.contactID1->GetVel()) );
	//point.contactID1->SetNewVel(glm::vec3( point.contactID1->GetVel().x, -point.contactID1->GetVel().y, point.contactID1->GetVel().z ));
	auto e = 0.6f;
	
	if(point.contactID2) {
		
		point.contactID1->SetNewPos(point.contactID1->GetNewPos() + point.dist * colNormal);
		point.contactID1->SetNewVel((point.contactID1->GetVel() - (1.0f + e) * colNormal * glm::dot(colNormal, point.contactID1->GetVel())));
		point.contactID2->SetNewPos(point.contactID2->GetNewPos() - point.dist * colNormal);;
		point.contactID2->SetNewVel((point.contactID2->GetVel() - (1.0f + e) * colNormal * glm::dot(colNormal, point.contactID2->GetVel())));
		
	}else {

		//point.contactID1->SetNewPos(point.plane.pointOfImpact - (point.contactID1->GetRadius() + 0.01f) * point.contactNormal);
		point.contactID1->SetNewVel((point.contactID1->GetNewVel() - (1.0f + e) * colNormal * glm::dot(colNormal, point.contactID1->GetNewVel())));
		point.contactID1->SetNewPos(point.pointOfImpact + point.timeAfterCollision * point.contactID1->GetNewVel() + point.contactID1->GetRadius()  * point.contactNormal);
		

		
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

	const glm::vec3 gravity(0.0f, -9.81f , 0.0f);
	const float k = 10; //k*pos thrust
	const float b = 1.0f;  //drag coefficient
	return  - b * state.velocity  + gravity *m_mass ;

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


}


Derivative Sphere::Evaluate(const State &initial, const float t, const float dt, const Derivative &d) const {

	State state;
	state.position = initial.position + d.dx * dt;
	state.velocity = initial.velocity + d.dv * dt;

	Derivative output;

	output.dx = state.velocity;
	output.dv = force(state, dt);

	return output;
}



