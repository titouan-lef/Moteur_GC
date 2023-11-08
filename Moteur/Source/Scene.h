#pragma once
#include <vector>

class Entity;

class Scene
{
public:
	Scene() { m_Initialized = false; }
	~Scene();

	virtual void Init() {}				// Called when scene is created (Add here your entities)
	virtual void Update() {}			// Called every frame, for your scene logical event 

	void RealUpdate();					// Called every frame by the engine, do not override this method
	void Render();						// Called every frame by the engine, do not override this method

	void AddEntity(Entity* entity);		// Add entity to scene
	void RemoveEntity(Entity* entity);	// Remove entity from scene
private:
	bool m_Initialized = false;			// Is scene initialized
protected:
	std::vector<Entity*> m_entities;	// List of entities in scene
};