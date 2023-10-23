#pragma once
#include <vector>

class Triangle
{
public:
	class Vertex
	{
	public:
		Vertex(float _x, float _y, float _z);
		~Vertex();

	private:
		float x, y, z;
	};

public:
	Triangle();//int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int _x3, int _y3, int _z3;
	~Triangle();

	//static size_t Size();
	//Vertex* GetTabVertices();

private:
	std::vector<Vertex> vertices = {
	{ 0.0f, 1.0f, 0.0f }, // Sommet 0
	{ 1.0f, -1.0f, 0.0f }, // Sommet 1
	{ -1.0f, -1.0f, 0.0f } // Sommet 2
	};
};

