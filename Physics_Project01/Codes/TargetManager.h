#ifndef _TARGETMANAGER_H_
#define _TARGETMANAGER_H_

#include "Base.h"

namespace Engine
{
	class CInputDevice;
	class CLayer;
}

class Player;
class SceneDungeon;

// A class that searches for and manages the target object to be moved by the user
class TargetManager : public Engine::CBase
{
	SINGLETON(TargetManager)

private:
	Engine::CInputDevice*		m_pInputDevice;
	Player*						m_pTarget;
	SceneDungeon*				m_pScene;
	Engine::CLayer*				m_pCharacterLayer;

private:
	explicit TargetManager();
	~TargetManager();
public:
	void Destroy();
	RESULT Ready(SceneDungeon* pScene);

public:
	void Update(const _float& dt);

private:
	void KeyCheck(const _float& dt);
	_bool FindTargetAndSet();
	void MoveTarget(_bool bSnap);
	void ReleaseTarget();
};

#endif //_TARGETMANAGER_H_