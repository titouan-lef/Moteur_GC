#pragma once

#include "framwork.h"

class Entity;

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager();

	static CollisionManager* GetInstance() { static CollisionManager cm; return &cm; }

	static void AddEntity(Entity* entity);
	static void RemoveEntity(Entity* entity);

	void Update();
private:
	std::vector<Entity*> _entityList;
};