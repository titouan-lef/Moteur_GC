#include "Icosahedron.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "ShaderColor.h"
#include <vector>
#include <cmath>

Icosahedron::Icosahedron()
{
    CreateVertices();
    CreateIndices();

    this->AddComponent<Transform>();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderColor::GetInstance());
}

void Icosahedron::CreateVertices() {
    const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;

    // The vertices of an icosahedron can be derived from three orthogonal golden rectangles
    m_vertices = {
        // (0, ±1, ±φ)
        Vertex{{ 0.0f, -1.0f, -phi }, {1.0f,1.0f}},
        Vertex{{ 0.0f,  1.0f, -phi }, {1.0f,1.0f}},
        Vertex{{ 0.0f, -1.0f,  phi }, {1.0f,1.0f}},
        Vertex{{ 0.0f,  1.0f,  phi }, {1.0f,1.0f}},

        // (±1, ±φ, 0)
        Vertex{{-1.0f, -phi,  0.0f }, {1.0f,1.0f}},
        Vertex{{ 1.0f, -phi,  0.0f }, {1.0f,1.0f}},
        Vertex{{-1.0f,  phi,  0.0f }, {1.0f,1.0f}},
        Vertex{{ 1.0f,  phi,  0.0f }, {1.0f,1.0f}},

        // (±φ, 0, ±1)
        Vertex{{-phi,  0.0f, -1.0f }, {1.0f,1.0f}},
        Vertex{{ phi,  0.0f, -1.0f }, {1.0f,1.0f}},
        Vertex{{-phi,  0.0f,  1.0f }, {1.0f,1.0f}},
        Vertex{{ phi,  0.0f,  1.0f }, {1.0f,1.0f}},
    };

    // Normalize the vertices so that they lie on the unit sphere
    for (Vertex& vertex : m_vertices) {
        float length = std::sqrt(vertex.m_position.x * vertex.m_position.x +
            vertex.m_position.y * vertex.m_position.y +
            vertex.m_position.z * vertex.m_position.z);
        vertex.m_position.x /= length;
        vertex.m_position.y /= length;
        vertex.m_position.z /= length;
    }
}

void Icosahedron::CreateIndices() {
    // An icosahedron consists of 20 triangles, which can be grouped into 5 sets
    // around each vertex of the upper and lower pentagon.

    m_indices = {
        // 5 faces around vertex 0
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        // 5 adjacent faces around vertices 2, 4, 6, 8, 9 for the upper half
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        // 5 faces around vertex 3 (opposite vertex 0)
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        // 5 adjacent faces around vertices 2, 4, 6, 7, 11 for the lower half
        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };
}