#pragma once

#include "framwork.h"

class Entity;

class CollisionManager
{
public:
	CollisionManager() { _instance = this; }
	virtual ~CollisionManager() { delete[] _instance; }

	static CollisionManager* _instance;
	static CollisionManager* GetInstance() { return _instance; }

	static void AddEntity(Entity* entity);
	static void RemoveEntity(Entity* entity);

	void Update();
private:
	std::vector<Entity*> _entityList;
};