#pragma once 
#include "Entity.h"
#include "Mesh.h"

class Cube : public Entity
{
public:
	Cube();
	~Cube();

    virtual void Update() override;
	
    /*void Update() override;
	void PostUpdate() override;
	void Render() override;
	void OnDestroy() override;*/

private:
	std::vector<Vertex> m_vertices = {
        // Front face
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},  // 0
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},   // 1
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},    // 2
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},   // 3

        // Back face
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},   // 4
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},    // 5
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},     // 6
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},    // 7
    };
	std::vector<UINT16> m_indices = {
        // Front face
        0, 3, 2, 2, 1, 0,

        // Back face
        4, 7, 6, 6, 5, 4,

        // Left face
        0, 4, 7, 7, 3, 0,

        // Right face
        1, 2, 6, 6, 5, 1,

        // Top face
        3, 7, 6, 6, 2, 3,

        // Bottom face
        0, 1, 5, 5, 4, 0
    };
};