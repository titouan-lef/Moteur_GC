#include "../header/framwork.h"
#include "../header/GameObject.h"
#include "../header/WindowManager.h"

GameObject::GameObject(float x, float y, float z, float width, float height, float depth) :
	m_x(x),
	m_y(y),
	m_z(z),
	m_width(width),
	m_height(height),
	m_depth(depth)
{
    float halfWidth = m_width / 2;
    float halfHeight = m_height / 2;

	m_vertices = {
        // Carr�
        { { x - halfWidth, y + halfHeight, z }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur gauche
        { { x + halfWidth, y + halfHeight, z }, { 0.0f, 1.0f, 0.0f, 1.0f } },// Coin sup�rieur droit
        { { x - halfWidth, y - halfHeight, z }, { 0.0f, 0.0f, 1.0f, 1.0f } },// Coin inf�rieur gauche

        { { x - halfWidth, y - halfHeight, z }, { 0.0f, 0.0f, 1.0f, 1.0f } },// Coin inf�rieur gauche
        { { x + halfWidth, y + halfHeight, z }, { 0.0f, 1.0f, 0.0f, 1.0f } },// Coin sup�rieur droit
        { { x + halfWidth, y - halfHeight, z }, { 1.0f, 1.0f, 0.0f, 1.0f } },// Coin inf�rieur droit

        // Face Sup�rieure
        { { x - halfWidth, y + halfHeight, z + depth }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur gauche
        { { x + halfWidth, y + halfHeight, z + depth }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur droit
        { { x - halfWidth, y + halfHeight, z }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin inf�rieur gauche

        { { x - halfWidth, y + halfHeight, z }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin inf�rieur gauche
        { { x + halfWidth, y + halfHeight, z + depth }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur droit
        { { x + halfWidth, y + halfHeight, z }, { 0.0f, 1.0f, 0.0f, 1.0f } }// Coin inf�rieur droit
    };

    WindowManager::AddGameObject(this);
}

GameObject::~GameObject()
{
}

std::vector<Vertex> GameObject::GetVertices()
{
    return m_vertices;
}


//m_vertices = {
 //       { { 0, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur gauche
 //       { { 0.5f, 0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Coin sup�rieur droit
 //       { { 0, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Coin inf�rieur gauche

 //       { { 0, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Coin inf�rieur gauche
 //       { { 0.5f, 0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Coin sup�rieur droit
 //       { { 0.5f, -0.25f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },  // Coin inf�rieur droit

 //       { { -0.75f, -0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },// Coin sup�rieur gauche
 //       { { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Coin sup�rieur droit
 //       { { -0.75f, -0.75f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Coin inf�rieur gauche

 //       { { -0.75f, -0.75f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Coin inf�rieur gauche
 //       { { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Coin sup�rieur droit
 //       { { -0.25f, -1 * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }  // Coin inf�rieur droit
 //   };