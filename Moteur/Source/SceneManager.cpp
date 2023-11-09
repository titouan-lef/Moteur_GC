#include "framwork.h"
#include "Scene.h"
#include "SceneManager.h"

SceneManager::~SceneManager()
{
	for (Scene* scene : m_Scenes)
	{
		delete scene;
		m_Scenes.erase(m_Scenes.begin());
	}
	m_pCurrentScene = nullptr;
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

void SceneManager::CurrentSceneUpdate()
{
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->RealUpdate();
	}
	else
	{
		std::cout << "No scene active" << std::endl;
		std::cout << "Scene list size: " << m_Scenes.size() << std::endl;
		if (m_Scenes.size() > 0)
		{
			m_pCurrentScene = m_Scenes[0];
		}
	}
}

void SceneManager::CurrentSceneRender()
{
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->Render();
	}
	else
	{
		std::cout << "No scene active" << std::endl;
		std::cout << "Scene list size: " << m_Scenes.size() << std::endl;
		if (m_Scenes.size() > 0)
		{
			m_pCurrentScene = m_Scenes[0];
		}
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

void SceneManager::FrameEnd()
{
	m_pCurrentScene->KillThemAll();
}
