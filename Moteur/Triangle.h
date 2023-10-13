#pragma once

class Triangle
{
public:
	Triangle(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int _x3, int _y3, int _z3);
	~Triangle();

private:
	class Vertex
	{
	public:
		Vertex(int _x, int _y, int _z);
		~Vertex();

	private:
		int x, y, z;
	};

	Vertex tabVertices[3];
};

