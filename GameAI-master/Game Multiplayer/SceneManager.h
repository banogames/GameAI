#pragma once
#include "Scene.h"

class SceneManager
{
	static SceneManager *_instance;
	Scene *_currentScene;

private:
	SceneManager();
public:
	static SceneManager* Instance();
	~SceneManager() {}
	Scene* GetCurrentScene();
	void ReplaceScene(Scene* scene);
};

