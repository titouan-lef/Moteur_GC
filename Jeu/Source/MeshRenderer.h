#pragma once
#include "Mesh.h"
#include "Shader.h"

class Component;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Init(Mesh* mesh, Shader* shader);

private:
	std::string m_shaderPath;

public://TO DO : a mettre en private ?
	Mesh* m_mesh = nullptr;
	Shader* m_shader = nullptr;
};