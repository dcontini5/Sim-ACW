#include "gl.h"
#include <GLFW/glfw3.h>
#include "Game.h"

Game::Game() : m_previousTime(0) {

	CreateSphereGeometry(_sphereGeometry.vertices, _sphereGeometry.indices);
	CreateCylinderGeometry(_cylinderGeometry.vertices, _cylinderGeometry.indices);

	_cylinder = new Mesh(_cylinderGeometry.vertices, _cylinderGeometry.indices);

	_sphereList.push_back(new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices, { { 0, 15, }, { 0, -5 } }));
	_sphereList.push_back(new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices, { { 0, 0, }, { 0.5f, -0 } }));
	_sphereList.push_back(new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices, { { 0, -15, }, { -1.0f, -20 } }));
	
	//m_sphere1 = new Sphere(_sphereGeometry.vertices , _sphereGeometry.indices);
	//m_sphere1->SetPos(0, 15);
	//m_sphere1->SetVel(0, -5);
	//m_sphere1->SetMass(750.0f);

	//m_sphere2 = new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices);
	//m_sphere2->SetPos(0, 0);
	//m_sphere2->SetVel(0.5, 0);
	//m_sphere2->SetMass(1000.0f);

	//m_sphere3 = new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices);
	//m_sphere3->SetPos(0, -15);
	//m_sphere3->SetVel(-1.0, 20);
	//m_sphere3->SetMass(2000.0f);

	auto tmp = CreateBoxGeometry();
	_box = new Mesh(tmp.vertices, tmp.indices);

	tmp = CreateTrayGeometry();
	_bottomTray = new Mesh(tmp.vertices, tmp.indices);
	tmp = CreateTopTrayGeometry();
	_topTray = new Mesh(tmp.vertices, tmp.indices);

	std::vector<unsigned int>::const_iterator first = _sphereGeometry.indices.begin() + 918;
	std::vector<unsigned int>::const_iterator last = _sphereGeometry.indices.end();
	std::vector<unsigned int> newVec(first, last);


	_bowl = new Mesh(_sphereGeometry.vertices, newVec);

	m_manifold = new ContactManifold();
	m_shader_program = new ShaderProgram("VS.glsl", "FS.glsl");
	
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);


	_model = glm::mat4(1.0f);
	_model = glm::rotate(_model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	_proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 200.0f);

	_cameraPos = glm::vec3(0.0f, 0.0f, 50.0f);
	_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	UpdateView();
	
	
}

Game::~Game(void)
{
	//delete m_sphere1;
	//delete m_sphere2;
	//delete m_sphere3;
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

void Game::CalculateObjectPhysics()
{

	for (auto i : _sphereList) i->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
	
	//m_sphere1->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
	//m_sphere2->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
	//m_sphere3->CalculatePhysics(static_cast<float>(start.QuadPart), m_dt);
}


//**************************Handle dynamic collisions***********************
void Game::DynamicCollisionDetection()
{
	auto pos = 1;
	for (auto i : _sphereList) {
		std::vector<Sphere*>::const_iterator first = _sphereList.begin() + pos;
		std::vector<Sphere*>::const_iterator last = _sphereList.end();
		std::vector<Sphere*> newVec(first, last);
		for (auto j : newVec) i->CollisionWithSphere( j, m_manifold);
		pos++;
	}
	
	//m_sphere1->CollisionWithSphere(m_sphere2, m_manifold);
	//m_sphere1->CollisionWithSphere(m_sphere3, m_manifold);
	//m_sphere2->CollisionWithSphere(m_sphere3, m_manifold);
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
	for (auto i : _sphereList) i->Update();
	//m_sphere1->Update();
	//m_sphere2->Update();
	//m_sphere3->Update();
}

//**************************Render and display the scene in OpenGL***********************
void Game::Render()									// Here's Where We Do All The Drawing
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw triangle
	glUseProgram(m_shader_program->getProgram());
	
	m_shader_program->setMat4("model", _model);
	m_shader_program->setMat4("view", _view);
	m_shader_program->setMat4("projection", _proj);
	glm::mat4 trans = glm::mat4(1);

	//draw ball

	for (auto i : _sphereList) i->Render(m_shader_program, glm::vec3(i->GetNewPos().GetX(), i->GetNewPos().GetY(), 0.0f ));

	
	//m_sphere1->Render(m_shader_program, glm::vec3(m_sphere1->GetNewPos().GetX(), m_sphere1->GetNewPos().GetY(), 0.0f));
	//m_sphere2->Render(m_shader_program, glm::vec3(m_sphere2->GetNewPos().GetX(), m_sphere2->GetNewPos().GetY(), 0.0f ));
	//m_sphere3->Render(m_shader_program, glm::vec3(m_sphere3->GetNewPos().GetX(), m_sphere3->GetNewPos().GetY(), 0.0f ));

	_box->Render(m_shader_program, glm::vec3(1.0f));
	_bottomTray->Render(m_shader_program, glm::vec3(1.0f));
	_topTray->Render(m_shader_program, glm::vec3(1.0f));
	
	

	
	trans = glm::translate(trans, glm::vec3(1.0f, 2.0f, 1.0f));
	trans = glm::rotate_slow(trans, static_cast<float>(end.QuadPart*0.0000001), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::scale(trans, glm::vec3(2.0f));
	_cylinder->Render(m_shader_program, trans);

	trans = glm::mat4(1);
	trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 1.0f));
	trans = glm::rotate_slow(trans, static_cast<float>(end.QuadPart*0.0000001), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::translate(trans, glm::vec3(-2.0f, 4.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_cylinder->Render(m_shader_program, trans);

	trans = glm::mat4(1);
	trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 1.0f));
	trans = glm::rotate_slow(trans, static_cast<float>(end.QuadPart*0.0000001), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::translate(trans, glm::vec3(-2.0f, 4.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_cylinder->Render(m_shader_program, trans);

	trans = glm::mat4(1);
	trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 1.0f));
	trans = glm::rotate_slow(trans, static_cast<float>(end.QuadPart*0.0000001), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::translate(trans, glm::vec3(-2.0f, 4.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	_cylinder->Render(m_shader_program, trans);

	trans = glm::mat4(1);
	trans = glm::translate(trans, glm::vec3( 1.0f, -10.0f, 1.0f));
	trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::scale(trans, glm::vec3( 25.0f, 25.0f, 10.0f ));
	_bowl->Render(m_shader_program, trans);

}

// Code taken from http://www.songho.ca/opengl/gl_sphere.html
void Game::CreateSphereGeometry(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {

	auto radius = 0.5;
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

void Game::CreateCylinderGeometry(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){


	const auto radius = 0.25f;
	const auto slicesize = 2 * glm::pi<float>()/18;

	for (auto y = 1; y > -2; y-=2) {


		for (auto z = 0; z < 18; z++) {



			vertices.push_back({ glm::vec3( radius * cos(slicesize * z), 1.5 * y, radius * sin(slicesize * z)) });


		}

	}


	for (auto i = 0; i < 18; i++) {

		
		indices.push_back(18 + i);
		indices.push_back( i );
		indices.push_back(18 + (17 + i) % 18);
		indices.push_back( i );
		indices.push_back((17 + i) % 18);
		indices.push_back(18 + (17 + i) % 18);


	}

}

Geometry Game::CreateBoxGeometry(){

	Geometry geometry;

	geometry.vertices.push_back({ glm::vec3(-5.0f, 10.0f, -5.0f) });
	geometry.vertices.push_back({ glm::vec3(-5.0f, 10.0f,  5.0f) });
	geometry.vertices.push_back({ glm::vec3( 5.0f, 10.0f,  5.0f) });
	geometry.vertices.push_back({ glm::vec3( 5.0f, 10.0f, -5.0f) });

	geometry.vertices.push_back({ glm::vec3(-5.0f, -10.0f, -5.0f) });
	geometry.vertices.push_back({ glm::vec3(-5.0f, -10.0f,  5.0f) });
	geometry.vertices.push_back({ glm::vec3( 5.0f, -10.0f,  5.0f) });
	geometry.vertices.push_back({ glm::vec3( 5.0f, -10.0f, -5.0f) });

	for (auto i = 0; i < 4; i++) {

		geometry.indices.push_back(4 + i);
		geometry.indices.push_back(0 + i);
		geometry.indices.push_back(4 + (3 + i) % 4);
		geometry.indices.push_back(0 + i);
		geometry.indices.push_back((3 + i) % 4);
		geometry.indices.push_back(4 + ( 3 + i) % 4);

	}

	return geometry;

}

Geometry Game::CreateTrayGeometry()
{
	Geometry geometry;

	auto y = -8.0f;

	geometry.vertices.push_back({ glm::vec3(-5.0f, y, -5.0f) });
	geometry.vertices.push_back({ glm::vec3(-5.0f, y,  5.0f) });
	geometry.vertices.push_back({ glm::vec3(5.0f,  y,  5.0f) });
	geometry.vertices.push_back({ glm::vec3(5.0f,  y, -5.0f) });

	geometry.indices.push_back(0);
	geometry.indices.push_back(1);
	geometry.indices.push_back(3);
	geometry.indices.push_back(1);
	geometry.indices.push_back(2);
	geometry.indices.push_back(3);
	
	return geometry;
}

Geometry Game::CreateTopTrayGeometry() {

	Geometry geometry;

	for (auto z = -5; z <= 5; z++) {

		for (auto x = -5; x < 5; x += 3) {

			geometry.vertices.push_back({ glm::vec3(x, -2.0f, z) });
			geometry.vertices.push_back({ glm::vec3(x + 1, -2.0f, z) });
			
		}

		
	}

	
	for(auto i = 1; i <= 10; i++) {

		for(auto j = 0; j<7; j++) {


			if ((i == 2 || i == 3) && (j == 1 || j == 3 || j == 5 )) continue;
			if ((i == 5 || i == 6) && (j == 1 || j == 5 )) continue;
			if ((i == 8 || i == 9) && (j == 1 || j == 3 || j == 5)) continue;
			geometry.indices.push_back(j + 8 * i);
			geometry.indices.push_back(j + 8 * (i - 1));
			geometry.indices.push_back(j + 8 * i + 1);
			geometry.indices.push_back(j + 8 * (i - 1));
			geometry.indices.push_back(j + 8 * (i - 1) + 1);
			geometry.indices.push_back(j + 8 * i + 1);

		}

		
	}

	auto circleStart = geometry.vertices.size();
	auto indexgap = 0;
	auto cz = -3;
	
	for (auto i = 0; i < 9; i++) {

		const auto cx = 3 * i % 9 - 3;
		
		const auto pi = glm::pi<float>() * 2 / 16;

		if (!cx && !cz) {

			indexgap += 2;
			continue;
			
		}

		for (auto j = 0; j < 16; j++) {

			geometry.vertices.push_back({ glm::vec3(cx + cos(pi*j), -2.0f, cz + sin(pi*j)) });

		}
		
		for (auto j = 0; j < 4; j++) {

			geometry.indices.push_back(26 + indexgap);
			geometry.indices.push_back(circleStart + j);
			geometry.indices.push_back(circleStart + j + 1);

			geometry.indices.push_back(25 + indexgap);
			geometry.indices.push_back(circleStart + j + 4);
			geometry.indices.push_back(circleStart + j + 5);

			geometry.indices.push_back(9 + indexgap);
			geometry.indices.push_back(circleStart + j + 8);
			geometry.indices.push_back(circleStart + j + 9);

			geometry.indices.push_back(10 + indexgap);
			geometry.indices.push_back(circleStart + j + 12);
			geometry.indices.push_back(circleStart + (j + 13) % 16);

		}

		if (cx == 3) {
			cz = (cz + 6) % 9 - 3;
			circleStart += 16;
			indexgap += 20;
			continue;
		}
		
		circleStart += 16;
		indexgap += 2;

	}

	return geometry;
}

void Game::AddBall(){

	_sphereList.push_back(new Sphere(_sphereGeometry.vertices, _sphereGeometry.indices, {{0, 10},{0, -5}}));
	
}
