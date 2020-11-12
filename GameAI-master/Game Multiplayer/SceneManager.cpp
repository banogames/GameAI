#include "SceneManager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager()
{
	_currentScene = nullptr;
}

SceneManager* SceneManager::Instance()
{
	if (_instance == nullptr)
		_instance = new SceneManager();

	return _instance;
}

Scene* SceneManager::GetCurrentScene()
{
	return _currentScene;
}

void SceneManager::ReplaceScene(Scene *scene)
{
	delete _currentScene;

	_currentScene = scene;
}