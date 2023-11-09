#pragma once
#include "Component.h"

using EventCallback = std::function<void(Entity* e)>;

class Transform;

// This component is add to an entity to make it collidable
// You can add a callback function to the collider that will be triggered on collision with other colliders
// please 
class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	virtual void Initialize()override;
	virtual void Update() override;

	Transform* GetCollider();
	// Set the tag of the collider for collision filtering
	void SetTag(std::string tag) { m_tag = tag; }
	// Get the tag of the collider, it's "Default" by default
	std::string GetTag() { return m_tag; }

	bool CheckCollision(Collider* collider);	// Check if the collider is colliding with another collider

	// You'll use this function to add a function to the callback list that will be triggered on collision with other colliders
	// You'll need to use std::bind to bind the function to the callback
	// Example: collider->addListener(std::bind(&ClassName::OnCollision, this, std::placeholders::_1));
	// collider is the collider that you want to add the callback to
	// "&ClassName::OnCollision" is the function you want to bind
	// "this" is the objetcs that contains the function, it will still be "this"
	// "std::placeholders::_1" is the parameter that the function takes, it will still be std::placeholders::_1
	void addListener(EventCallback callback);

	//Trigger all the functions in the callback list
	void triggerEvent(Entity* e);	
private:
	Transform* m_Collider;
	std::string m_tag;

	std::vector<EventCallback> m_callbacks;
};