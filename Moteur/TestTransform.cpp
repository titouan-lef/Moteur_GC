#include "TestTransform.h"


int RunTest()
{
	Entity* e = new Entity();

	// on veut tester si la transformation marche bien
	// on va donc faire une transformation de 1, 2, 3
	// puis on va la faire tourner de 90 degres

	std::cout << "X : " << e->m_Transform.GetPosition().x;
	std::cout << "Y : " << e->m_Transform.GetPosition().y;
	std::cout << "Z : " << e->m_Transform.GetPosition().z << std::endl;

	e->m_Transform.SetPosition(1.0f, 2.0f, 3.0f);

	std::cout << "X : " << e->m_Transform.GetPosition().x;
	std::cout << "Y : " << e->m_Transform.GetPosition().y;
	std::cout << "Z : " << e->m_Transform.GetPosition().z << std::endl;


	return 0;
}
