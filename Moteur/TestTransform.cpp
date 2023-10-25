#include "TestTransform.h"

int RunTests()
{
	Entity* entity = new Entity();

	TestSetPos(entity);
	TestMove(entity);
	TestRotate(entity);
	TestScale(entity);

	return 0;
}

void TestSetPos(Entity* e)
{
	std::cout << "Test SetPosition" << std::endl;
	std::cout << "X : " << e->m_Transform.GetPosition().x << " Y : " << e->m_Transform.GetPosition().y << " Z : " << e->m_Transform.GetPosition().z << std::endl;

	e->m_Transform.SetPosition(1.0f, 2.0f, 3.0f);

	std::cout << " X : " << e->m_Transform.GetPosition().x << " Y : " << e->m_Transform.GetPosition().y << " Z : " << e->m_Transform.GetPosition().z << std::endl;
}

void TestMove(Entity* e)
{
	std::cout << "Test Move" << std::endl;
	std::cout << "X : " << e->m_Transform.GetPosition().x << " Y : " << e->m_Transform.GetPosition().y << " Z : " << e->m_Transform.GetPosition().z << std::endl;

	e->m_Transform.MoveByVector(XMFLOAT3(1.0f, 2.0f, 3.0f));

	std::cout << " X : " << e->m_Transform.GetPosition().x << " Y : " << e->m_Transform.GetPosition().y << " Z : " << e->m_Transform.GetPosition().z << std::endl;
}

void TestRotate(Entity* e)
{
	std::cout << "Test Rotate" << std::endl << std::endl;
	std::cout << e->m_Transform.GetRotationMatrix() << std::endl << std::endl;

	e->m_Transform.Rotate(1.0f, 2.0f, 3.0f);

	std::cout << e->m_Transform.GetRotationMatrix() << std::endl << std::endl;
}

void TestScale(Entity* e)
{
	std::cout << "Test Scale" << std::endl;
	std::cout << "X : " << e->m_Transform.GetScale().x << " Y : " << e->m_Transform.GetScale().y << " Z : " << e->m_Transform.GetScale().z << std::endl;

	e->m_Transform.SetScale(10.0f);

	std::cout << "X : " << e->m_Transform.GetScale().x << " Y : " << e->m_Transform.GetScale().y << " Z : " << e->m_Transform.GetScale().z << std::endl;
}

void TestMatrix(Entity* e)
{
	std::cout << "Test Matrix Update" << std::endl;
	std::cout << e->m_Transform.GetMatrix() << std::endl << std::endl;

	e->m_Transform.UpdateMatrix();

	std::cout << e->m_Transform.GetMatrix() << std::endl << std::endl;
}

void TestQuaternion(Entity* e)
{
	
}








