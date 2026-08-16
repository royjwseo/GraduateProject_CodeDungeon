#pragma once

enum class SCENE_STATE
{
	LOBBY,
	PLAYING
};

class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		static SceneManager sm;
		return &sm;
	}
public:
	SCENE_STATE Cur_SceneState;
	SCENE_STATE GetSceneState() { return Cur_SceneState; }
	
};

