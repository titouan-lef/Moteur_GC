#pragma once 

#include "Entity.h"
#include "framwork.h"

class Rectangle : public Entity
{
public:
	Rectangle();
	~Rectangle();

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void Render() override;
	void OnDestroy() override;

private:
};