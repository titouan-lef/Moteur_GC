#include <Timer.h>
#include "Controller.h"
#include "Bullet.h"
#include "Shooter.h"

Shooter::Shooter()
{
}

Shooter::~Shooter()
{
}

void Shooter::Initialize()
{
	timer = new Timer();
}

void Shooter::Update()
{
	if (timer->Peek() < cooldown) return;
	if (controller->IsMoving(Controller::Direction::LeftClick))
	{
		Bullet* bullet = new Bullet(controller->m_coordMouse.x, controller->m_coordMouse.y);
		this->GetOwner()->AddChild(bullet);
		timer = new Timer();
	}
}
