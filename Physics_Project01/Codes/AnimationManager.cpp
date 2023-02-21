#include "AnimationManager.h"
#include "Animator.h"
#include "InputDevice.h"
#include "Player.h"


SINGLETON_FUNCTION(AnimationManager)
USING(Engine)
USING(std)

AnimationManager::AnimationManager()
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
	m_vecAnimators.clear();

	Ready();
}

AnimationManager::~AnimationManager()
{
}

// Call instead of destructor to manage class internal data
void AnimationManager::Destroy()
{
	SafeDestroy(m_pInputDevice);

	for (int i = 0; i < m_vecAnimators.size(); ++i)
		SafeDestroy(m_vecAnimators[i]);
	m_vecAnimators.clear();
}

// Initialize
RESULT AnimationManager::Ready()
{
	return PK_NOERROR;
}

// Basic Update Function, update all animators
void AnimationManager::Update(const _float& dt)
{
	for (int i = 0; i < m_vecAnimators.size(); ++i)
	{
		if (m_vecAnimators[i]->GetIsPlaying())
			m_vecAnimators[i]->FrameMove(dt);
	}
}

// Add Animator
void AnimationManager::AddAnimator(Animator* pAnimator)
{
	m_vecAnimators.push_back(pAnimator);
}

