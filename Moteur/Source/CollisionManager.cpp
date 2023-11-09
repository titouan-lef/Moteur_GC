#include "Collider.h"
#include "Entity.h"
#include "CollisionManager.h"
#include "Engine.h"

CollisionManager::~CollisionManager()
{
}

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager cm;
	return &cm;
}

void CollisionManager::AddEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		GetInstance()->_entityList.push_back(entity);
	}
}

void CollisionManager::RemoveEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		for (int i = 0; i < GetInstance()->_entityList.size(); i++)
		{
			if (GetInstance()->_entityList[i] == entity)
			{
				GetInstance()->_entityList.erase(GetInstance()->_entityList.begin() + i);
			}
		}
	}
}

void CollisionManager::Update()
{
	XMVECTOR playerVector;
	if (Engine::GetInstance()->GetPlayer() != nullptr)
		playerVector = XMLoadFloat3(Engine::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetPosition());
	else
		return;

	for (int i = 0; i < _entityList.size(); i++)
	{
		XMVECTOR vector = XMLoadFloat3(_entityList[i]->GetComponent<Transform>()->GetPosition());
		XMVECTOR distanceVector = XMVector3Length(XMVectorSubtract(playerVector, vector));
		float distance;
		XMStoreFloat(&distance, distanceVector);

		bool distFromPlayer =  distance > 3;
		if (!_entityList[i]->GetComponent<Transform>()->IsOnScreen() || distFromPlayer)
			continue;

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
					collider1->triggerEvent(_entityList[j]);
					collider2->triggerEvent(_entityList[i]);
				}
			}
		}
	}
}
