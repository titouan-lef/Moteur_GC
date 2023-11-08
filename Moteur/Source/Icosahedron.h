#pragma once
#include "Entity.h"
#include "Mesh.h"
class Icosahedron : public Entity
{
public:
	Icosahedron();
	void CreateVertices();
	void CreateIndices();
private:
	std::vector<Vertex> m_vertices;
	std::vector<UINT16> m_indices;
};

