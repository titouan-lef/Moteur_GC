#include "CollisionManagrer.h"
#include "Collider.h"
#include "Entity.h"

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
	for (int i = 0; i < _entityList.size(); i++)
	{
		Collider* collider1 = _entityList[i]->GetComponent<Collider>();
		if (!collider1->IsActive()) continue;
		for (int j = i + 1; j < _entityList.size(); j++)
		{
			Collider* collider2 = _entityList[j]->GetComponent<Collider>();
			if (!collider2->IsActive()) continue;
			if (collider1 != nullptr && collider2 != nullptr)
			{
				if (collider1->CheckCollision(collider2))
				{
					collider1->OnCollision(_entityList[j]);
					collider2->OnCollision(_entityList[i]);
				}
			}
		}
	}
}
