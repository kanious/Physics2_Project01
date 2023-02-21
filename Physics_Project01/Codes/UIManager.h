#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "Base.h"

class SceneDungeon;
// Manages all UI
class UIManager : public Engine::CBase
{
	SINGLETON(UIManager)
private:
	SceneDungeon*		m_pScene;

private:
	explicit UIManager();
	~UIManager();
public:
	void Destroy();
	void RenderUI();
	RESULT Ready(SceneDungeon* pScene);
};

#endif //_UIMANAGER_H_