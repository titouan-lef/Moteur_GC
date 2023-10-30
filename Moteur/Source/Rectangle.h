#pragma once 

#include "Entity.h"
#include "framwork.h"

class MyRectangle : public Entity
{
public:
	MyRectangle();
	~MyRectangle();

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void Render() override;
	void OnDestroy() override;

private:
};