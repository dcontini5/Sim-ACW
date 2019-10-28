#include "Sphere.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"

struct Derivative;
struct State;
int Sphere::countID = 0;

Sphere::Sphere(void) : m_mass(1), m_radius(5)
{
	m_objectID = countID;
	++countID;
	m_texture = TextureLoader::LoadBMP("checker.bmp");
}

Sphere::~Sphere(void)
{
}

void Sphere::SetPos(float x, float y)
{
	m_pos.Set(x, y);
}

void Sphere::SetNewPos(Vector2f pos)
{
	m_newPos = pos;
}

void Sphere::SetVel(float x, float y)
{
	m_velocity.Set(x, y);
}

void Sphere::SetNewVel(Vector2f vel)
{
	m_newVelocity = vel;
}

void Sphere::SetMass(float mass)
{
	m_mass = mass;
}

void Sphere::CalculatePhysics(float dt)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	Vector2f force(0.0f, -9.81f * m_mass);

	// Calculate acceleration
	Vector2f accel = force / m_mass;

	// Integrate accel to get the new velocity (using Euler)
	m_newVelocity = m_velocity + (accel * dt);

	// Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	// Integration /w improved Euler
	m_newPos = m_pos + ((m_velocity + m_newVelocity) / 2 * dt);

	// Integration /w Runge-Kutta
	

	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model collisions with a plane       ********
	// ****************************************************************
	// ******** Replace with better collision detection code   ********
	// ****************************************************************
	if(m_newPos.GetY() < -20.0f+m_radius)
	{
		m_newPos.Set(m_newPos.GetX(), -20.0f + m_radius);
		m_newVelocity.Set(m_newVelocity.GetX(), 0.0f);
	}
}

//add Runge Kutta
/*void Sphere::CalculatePhysics(float dt, float t)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	Vector2f force(0.0f, -9.81f * m_mass);

	// Calculate acceleration
	Vector2f accel = force / m_mass;

	// Integrate accel to get the new velocity (using Euler)
	m_newVelocity = m_velocity + (accel * dt);

	// Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	// Integration /w improved Euler
	m_newPos = m_pos + ((m_velocity + m_newVelocity) / 2 * dt);

	// Integration /w Runge-Kutta
	

	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model collisions with a plane       ********
	// ****************************************************************
	// ******** Replace with better collision detection code   ********
	// ****************************************************************
	if(m_newPos.GetY() < -20.0f+m_radius)
	{
		m_newPos.Set(m_newPos.GetX(), -20.0f + m_radius);
		m_newVelocity.Set(m_newVelocity.GetX(), 0.0f);
	}
}*/

void Sphere::CollisionWithSphere(Sphere* sphere2, ContactManifold *contactManifold)
{

	Vector2f pos1 = this->GetNewPos();
	Vector2f pos2 = sphere2->GetNewPos();
	float dist = pos1.distance(pos2) - (this->GetRadius() + sphere2->GetRadius());
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;
		mp.contactID2 = sphere2;
		mp.contactNormal = (pos2 - pos1).normalise();
		contactManifold->Add(mp);
	}

}

void Sphere::ResetPos()
{
	m_newPos = m_pos;
}

void Sphere::Update()
{
	m_velocity = m_newVelocity;
	m_pos = m_newPos;
}

void Sphere::CollisionResponseWithSphere(ManifoldPoint &point)
{
	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model responses between Spheres     ********
	// ****************************************************************
	// ******** Replace with better response code for Spheres  ********
	// ****************************************************************

	Vector2f colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(-1.0f*colNormal*colNormal.dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(-1.0f*colNormal*colNormal.dot(point.contactID2->GetVel()));
}

float Sphere::GetMass() const
{
	return m_mass;
}

Vector2f Sphere::GetPos() const
{
	return m_pos;
}

Vector2f Sphere::GetNewPos() const
{
	return m_newPos;
}

Vector2f Sphere::GetVel() const
{
	return m_velocity;
}

Vector2f Sphere::GetNewVel() const
{
	return m_newVelocity;
}

float Sphere::GetRadius() const
{
	return m_radius;
}

void Sphere::Render() const									
{
	glPushMatrix();
		glTranslatef(m_pos.GetX(), m_pos.GetY(), 0);
		glColor3d(1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
		GLUquadric *quadric = gluNewQuadric();
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluSphere(quadric, m_radius, 20, 20);
		
	glPopMatrix();
}


Derivative Evaluate(const State &initial, float t, float dt, const Derivative &d);

