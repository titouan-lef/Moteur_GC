#pragma once

#include "framwork.h"

class Entity;

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager();

	static CollisionManager* GetInstance();

	static void AddEntity(Entity* entity);
	static void RemoveEntity(Entity* entity);

	void Update();
private:
	std::vector<Entity*> _entityList;
};