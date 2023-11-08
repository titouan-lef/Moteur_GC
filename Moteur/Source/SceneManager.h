#pragma once

class Scene;

class SceneManager
{
public:
	SceneManager() {}
	~SceneManager();

	static SceneManager* GetInstance();

	Scene* GetActiveScene();		// Get the current active scene

	template <typename T>// = Scene>
	inline void GoToScene();		// Go to a specific scene
	template <typename T = Scene>
	inline void AddScene();			// Add a scene to the scene manager
	template <typename T>// = Scene>
	inline void RemoveScene();		// Remove a scene from the scene manager

	void Update();
private:
	Scene* m_pCurrentScene = nullptr;
	std::vector<Scene*> m_Scenes;

	bool CkeckIfSceneExists(Scene* s); // Check if a scene exists in the scene manager
};

template<typename T>
void SceneManager::GoToScene()
{
	for (Scene* scene : m_Scenes)
	{
		if (typeid(*scene) == typeid(T))
		{
			m_pCurrentScene = dynamic_cast<Scene*>(scene);
			return;
		}
	}
}

template<typename T>
void SceneManager::AddScene()
{
	T* scene = new T();
	if (!CkeckIfSceneExists(scene))
		m_Scenes.push_back(scene);
}

template<typename T>
void SceneManager::RemoveScene()
{
	for (auto it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
	{
		if (typeid(*it) == typeid(T))
		{
			delete *it;
			m_Scenes.erase(it);
			return;
		}
	}
}