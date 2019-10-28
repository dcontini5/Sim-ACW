#include "ContactManifold.h"
#include "Sphere.h"
#include <assert.h>

ContactManifold::ContactManifold(void) : m_numOfPoints(0)
{
}


ContactManifold::~ContactManifold(void)
{
}

void ContactManifold::Add(ManifoldPoint point)
{
	m_points[m_numOfPoints] = point;
	++m_numOfPoints;
}

void ContactManifold::Clear()
{
	m_numOfPoints = 0;
}

int ContactManifold::GetNumPoints() const
{
	return m_numOfPoints;
}

ManifoldPoint& ContactManifold::GetPoint(int index)
{
	return m_points[index];
}