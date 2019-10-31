#include "Sphere.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include "TextureLoader.h"

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
	m_state.position.Set(x, y);
}

void Sphere::SetNewPos(Vector2f pos)
{
	m_newState.position = pos;
}

void Sphere::SetVel(float x, float y)
{
	m_state.velocity.Set(x, y);
}

void Sphere::SetNewVel(Vector2f vel)
{
	m_newState.velocity = vel;
}

void Sphere::SetMass(float mass)
{
	m_mass = mass;
}

#define RK 1
#if RK 1

void Sphere::CalculatePhysics(float t, float dt)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	//Vector2f force(0.0f, -9.81f * m_mass);

	// Calculate acceleration
	//Vector2f accel = force / m_mass;
	//Vector2f accel = acceleration(m_state, dt);

	// Integrate accel to get the new velocity (using Euler)
	//m_newVelocity = m_state.velocity + (accel * dt);

	// Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	// Integration /w improved Euler
	//m_newPos = m_state.position + ((m_state.velocity + m_newVelocity) / 2 * dt);

	// Integration /w Runge-Kutta
	integrate(m_state, t, dt);

	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model collisions with a plane       ********
	// ****************************************************************
	// ******** Replace with better collision detection code   ********
	// ****************************************************************
	//if(m_newPos.GetY() < -20.0f+m_radius)
	//{
	//	m_newPos.Set(m_newPos.GetX(), -20.0f + m_radius);
	//	m_newVelocity.Set(m_newVelocity.GetX(), 0.0f);
	//}
}


#else
void Sphere::CalculatePhysics(float dt)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	//Vector2f force(0.0f, -9.81f * m_mass);

	// Calculate acceleration
	//Vector2f accel = force / m_mass;
	//Vector2f accel = acceleration(m_state, dt);

	// Integrate accel to get the new velocity (using Euler)
	//m_newVelocity = m_state.velocity + (accel * dt);

	// Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	// Integration /w improved Euler
	//m_newPos = m_state.position + ((m_state.velocity + m_newVelocity) / 2 * dt);

	// Integration /w Runge-Kutta
	integrate(m_state, 0, dt);

	// ****************************************************************
	// ******** This implementation is very basic and does not ********
	// ******** accurately model collisions with a plane       ********
	// ****************************************************************
	// ******** Replace with better collision detection code   ********
	// ****************************************************************
	//if(m_newPos.GetY() < -20.0f+m_radius)
	//{
	//	m_newPos.Set(m_newPos.GetX(), -20.0f + m_radius);
	//	m_newVelocity.Set(m_newVelocity.GetX(), 0.0f);
	//}
}
#endif





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

void Sphere::Render() const									
{
	glPushMatrix();
		glTranslatef(m_state.position.GetX(), m_state.position.GetY(), 0);
		glColor3d(1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
		GLUquadric *quadric = gluNewQuadric();
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluSphere(quadric, m_radius, 20, 20);
		
	glPopMatrix();
}

Vector2f Sphere::force(const State& state, float t) {

	Vector2f force(0.0f, -9.81f * 3);
	const float k = 10;
	const float b = 1;
	return  ((-k * state.position - b * state.velocity )  / t) + force;
	
}

void Sphere::integrate(State& state, float t, float dt) {

	Derivative a, b, c, d;

	a = Evaluate(state, t, 0.0f, Derivative());
	b = Evaluate(state, t, dt*0.5f, a);
	c = Evaluate(state, t, dt*0.5f, b);
	d = Evaluate(state, t, dt, c);

	Vector2f dxdt = 1.0f / 6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);

	Vector2f dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

	m_newState.position = state.position + dxdt * dt;
	m_newState.velocity = state.velocity + dvdt * dt;
	/*state.position = state.position + dxdt * dt;
	state.velocity = state.velocity + dvdt * dt;*/
	
	if (m_newState.position.GetY() < -20.0f + m_radius)
	{
		m_newState.position.Set(state.position.GetX(), -20.0f + m_radius);
		m_newState.velocity.Set(state.velocity.GetX(), -state.velocity.GetY()-0.5);
		
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

