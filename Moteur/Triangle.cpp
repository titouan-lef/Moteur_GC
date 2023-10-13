#include "Triangle.h"

Triangle::Vertex::Vertex(int _x, int _y, int _z) : x(_x), y(_y), z(_z)
{
}

Triangle::Vertex::~Vertex()
{
}

Triangle::Triangle(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int _x3, int _y3, int _z3)
	:
	tabVertices{
		Vertex(_x1, _y1, _z1),
		Vertex(_x2, _y2, _z2),
		Vertex(_x3, _y3, _z3)
	}
{
}

Triangle::~Triangle()
{
}
