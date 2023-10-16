#pragma once

class Triangle
{
public:
	class Vertex
	{
	public:
		Vertex(int _x, int _y, int _z);
		~Vertex();

	private:
		int x, y, z;
	};

public:
	Triangle(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int _x3, int _y3, int _z3);
	~Triangle();

	static size_t Size();
	Vertex* GetTabVertices();

private:
	static const int nbVertices = 3;
	Vertex tabVertices[nbVertices];
};

