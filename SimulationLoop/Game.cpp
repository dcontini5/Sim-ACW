#include "gl.h"
#include <GLFW/glfw3.h>
#include "Game.h"
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\GLU.h>

Game::Game() : m_previousTime(0) {

	//m_sphere1 = new Sphere(vertices , indices);
	//m_sphere1->SetPos(0, 15);
	//m_sphere1->SetVel(0, -5);
	//m_sphere1->SetMass(750.0f);

	m_sphere2 = new Sphere(vertices, indices);
	m_sphere2->SetPos(0, 0);
	m_sphere2->SetVel(0.5, 0);
	m_sphere2->SetMass(1000.0f);

	m_sphere3 = new Sphere(vertices, indices);
	m_sphere3->SetPos(0, -15);
	m_sphere3->SetVel(-1.0, 20);
	m_sphere3->SetMass(2000.0f);

	m_manifold = new ContactManifold();
	m_shader_program = new ShaderProgram("VS.glsl", "FS.glsl");
	m_shader_program = new ShaderProgram("VS.glsl", "FS.glsl");
	
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	CreateSphereGeometry(_sphereGeometry.vertices, _sphereGeometry.indices);

	m_sphere1 = new Sphere(_sphereGeometry.vertices , _sphereGeometry.indices);
	m_sphere1->SetPos(0, 15);
	m_sphere1->SetVel(0, -5);
	m_sphere1->SetMass(750.0f);
	
}
//
//Game::Game(HDC hdc) : m_hdc(hdc), m_previousTime(0)
//{
//	m_sphere1 = new Sphere();
//	m_sphere1->SetPos(0, 15);
//	m_sphere1->SetVel(0,-5);
//	m_sphere1->SetMass(750.0f);
//
//	m_sphere2 = new Sphere();
//	m_sphere2->SetPos(0,0);
//	m_sphere2->SetVel(0.5,0);
//	m_sphere2->SetMass(1000.0f);
//
//	m_sphere3 = new Sphere();
//	m_sphere3->SetPos(0,-15);
//	m_sphere3->SetVel(-1.0, 20);
//	m_sphere3->SetMass(2000.0f);
//
//	m_manifold = new ContactManifold();
//
//	QueryPerformanceFrequency(&frequency);
//	QueryPerformanceCounter(&start);
//}

Game::~Game(void)
{
	delete m_sphere1;
	delete m_sphere2;
	delete m_sphere3;
	delete m_manifold;
}

void Game::Update()
{
	// **************************************************
	// The simulation loop should be on its own thread(s)
	// **************************************************
	SimulationLoop();
	Render();

}

void Game::SimulationLoop()
{


	
	// calculate dt based on the simulation loop rate using a timer
	QueryPerformanceCounter(&end);
	m_dt = static_cast<float>((end.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart));
	start = end;

	m_fps = static_cast<int>(1.0 / m_dt);

	// Calculate the physic calculations on all objects (e.g. new position, velocity, etc)
	CalculateObjectPhysics();

	// Clear the manifold so that we can calculate all collisions for this simulation loop
	m_manifold->Clear();

	// Find dynamic collisions for all objects and add to contact manifold 
	DynamicCollisionDetection();

	// Handle dynamic collision responses using the contact manifold
	DynamicCollisionResponse();

	// Update the physics calculations on all objects (e.g. new position, velocity, etc)
	UpdateObjectPhysics();
}


//**************************Update the physics calculations on each object***********************
#define RK 1
#if RK
void Game::CalculateObjectPhysics()
{
	m_sphere1->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
	m_sphere2->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
	m_sphere3->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
}
#else
void Game::CalculateObjectPhysics()
{
	m_sphere1->CalculatePhysics(m_dt);
	m_sphere2->CalculatePhysics(m_dt);
	m_sphere3->CalculatePhysics(m_dt);
}

#endif




//**************************Handle dynamic collisions***********************
void Game::DynamicCollisionDetection()
{
	m_sphere1->CollisionWithSphere(m_sphere2, m_manifold);
	m_sphere1->CollisionWithSphere(m_sphere3, m_manifold);
	m_sphere2->CollisionWithSphere(m_sphere3, m_manifold);
}

//**************************Handle dynamic collision responses***********************
void Game::DynamicCollisionResponse()
{
	for(int collision = 0; collision < m_manifold->GetNumPoints(); ++collision)
	{
		ManifoldPoint &point = m_manifold->GetPoint(collision);
		point.contactID1->CollisionResponseWithSphere(point);
	}
}

//**************************Update the physics calculations on each object***********************
void Game::UpdateObjectPhysics()
{
	m_sphere1->Update();
	m_sphere2->Update();
	m_sphere3->Update();
}

//**************************Render and display the scene in OpenGL***********************
#define GL1 0

#if GL1 

void Game::Render()									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	gluLookAt(0, 50, 100, 0, 0, 0, 0, 1, 0);

	glDisable(GL_TEXTURE_2D);
	// Draw plane (at y=-20)
	glBegin(GL_QUADS);
		glColor3d(1, 1, 1);
		glVertex3d(-50, -20, -50);
		glVertex3d( 50, -20, -50);
		glVertex3d( 50, -20,  50);
		glVertex3d(-50, -20,  50);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	m_sphere1->Render();
	m_sphere2->Render();
	m_sphere3->Render();

	SwapBuffers(m_hdc);				// Swap Buffers (Double Buffering)
}

#else

void Game::Render()									// Here's Where We Do All The Drawing
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw triangle
	glUseProgram(m_shader_program->getProgram());
	
	//draw ball
	
	m_sphere1->Render(m_shader_program, glm::vec3(0.5f));

	
	
}

#endif

// Code taken from http://www.songho.ca/opengl/gl_sphere.html
void Game::CreateSphereGeometry(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {

	auto radius = 1;
	auto sectorCount = 18;
	auto stackCount = 18;

	auto const pi = glm::pi<float>();

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * pi / sectorCount;
	float stackStep = pi / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back({ glm::vec3(x, y, z) });
		

			//// normalized vertex normal (nx, ny, nz)
			//nx = x * lengthInv;
			//ny = y * lengthInv;
			//nz = z * lengthInv;
			//normals.push_back(nx);
			//normals.push_back(ny);
			//normals.push_back(nz);

			//// vertex tex coord (s, t) range between [0, 1]
			//s = (float)j / sectorCount;
			//t = (float)i / stackCount;
			//texCoords.push_back(s);
			//texCoords.push_back(t);


		}
	}
	
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

}
