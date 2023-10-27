#pragma once

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();


private:
	std::string m_shaderPath;

};