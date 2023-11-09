#pragma once
#include "Component.h"

using EventCallback = std::function<void(Entity* e)>;

class Transform;

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	virtual void Initialize()override;
	virtual void Update() override;

	Transform* GetCollider();
	void SetTag(std::string tag) { m_tag = tag; }
	std::string GetTag() { return m_tag; }

	bool CheckCollision(Collider* collider);	// Check if the collider is colliding with another collider

	void addListener(EventCallback callback);	//Add a function to the callback list (need to have an Entity parameter like functionName(Entity* e))
	void triggerEvent(Entity* e);				//Trigger all the functions in the callback list	
private:
	Transform* m_Collider;
	std::string m_tag;

	std::vector<EventCallback> m_callbacks;
};