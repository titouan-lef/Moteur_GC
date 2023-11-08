#pragma once

class Scene;

class SceneManager
{
public:
	SceneManager() {}
	~SceneManager();

	static SceneManager* GetInstance();

	Scene* GetActiveScene() { return m_pCurrentScene; }

	void NextScene();
	void PreviousScene();

	template <typename T>// = Scene>
	void GoToScene(T* scene);
	template <typename T>// = Scene>
	void AddScene(T* scene);
	template <typename T>// = Scene>
	void RemoveScene(T* scene);

	void Update();
private:
	Scene* m_pCurrentScene;
	std::vector<Scene*> m_Scenes;

	bool CkeckIfSceneExists(Scene* s);
};