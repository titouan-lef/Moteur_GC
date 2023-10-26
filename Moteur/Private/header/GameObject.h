#pragma once

class GameObject
{
public:
	GameObject(float x, float y, float z, float width, float height, float depth);
	virtual ~GameObject();

	std::vector<Vertex> GetVertices();

private:
	float m_x, m_y, m_z;
	float m_width, m_height, m_depth;

	std::vector<Vertex> m_vertices = { {  } };
};

