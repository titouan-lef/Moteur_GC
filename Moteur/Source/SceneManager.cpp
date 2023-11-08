#include "framwork.h"
#include "Scene.h"
#include "SceneManager.h"

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

Scene* SceneManager::GetActiveScene()
{
	if (m_pCurrentScene == nullptr)
	{
		if (m_Scenes.size() > 0)
		{
			m_pCurrentScene = m_Scenes[0];
			return m_pCurrentScene;
		}
	} 
	else 
	{
		return m_pCurrentScene;
	}
}

void SceneManager::Update()
{
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->Update();
	}
	else
	{
		std::cout << "No scene active" << std::endl;
	}
}

bool SceneManager::CkeckIfSceneExists(Scene* s)
{
	for (auto scene : m_Scenes)
	{
		if (typeid(*scene) == typeid(*s))
		{
			return true;
		}
	}

	return false;
}
