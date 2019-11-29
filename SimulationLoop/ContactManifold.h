#pragma once

#include <glm/vec3.hpp>

class Sphere;


struct ManifoldPoint {
	Sphere *contactID1;
	Sphere *contactID2;
	glm::vec3 contactNormal;
	glm::vec3 pointOfImpact;
	float timeAfterCollision;
	float dist;
};

class ContactManifold
{
public:
	ContactManifold(void);
	~ContactManifold(void);
	void Add(const ManifoldPoint& point);
	void Clear();
	int GetNumPoints() const;
	ManifoldPoint& GetPoint(int index);
	
	
private:
	// ******************************************************************************************
	// The max number of points in the manifold is not verified in the code, so you need to test!
	// Maybe it would be safer to use a dyamic data sturcture (although this may be too slow)?
	// ******************************************************************************************
	ManifoldPoint m_points[1000];
	int m_numOfPoints;
	
};

