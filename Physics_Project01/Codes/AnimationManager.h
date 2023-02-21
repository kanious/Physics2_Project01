#ifndef _ANIMATIONMANAGER_H_
#define _ANIMATIONMANAGER_H_

#include "Base.h"

namespace Engine
{
	class CInputDevice;
}
class Player;
class Animator;

// A class that oversees the client's animator
class AnimationManager : public Engine::CBase
{
	SINGLETON(AnimationManager)

private:
	Engine::CInputDevice*			m_pInputDevice;
	std::vector<Animator*>			m_vecAnimators;

private:
	explicit AnimationManager();
	~AnimationManager();
public:
	void Destroy();
	RESULT Ready();

public:
	void Update(const _float& dt);

public:
	void AddAnimator(Animator* pAnimator);
};

#endif //_ANIMATIONMANAGER_H_