#include "CollisionManagrer.h"

void CollisionManager::AddEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		_instance->_entityList.push_back(entity);
	}
}

void CollisionManager::RemoveEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		for (int i = 0; i < _instance->_entityList.size(); i++)
		{
			if (_instance->_entityList[i] == entity)
			{
				_instance->_entityList.erase(_instance->_entityList.begin() + i);
			}
		}
	}
}

void CollisionManager::Update()
{
}
