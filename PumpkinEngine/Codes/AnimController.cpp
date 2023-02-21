#include "pch.h"
#include "..\Headers\AnimController.h"
#include "..\Headers\Animation.h"
#include <functional>


USING(Engine)
USING(std)
USING(glm)

CAnimController::CAnimController()
	: m_pCurAnimation(nullptr), m_iFrameIndex(0), m_fCurTime(0.f), m_fAnimSpeed(1.f)
	, m_bReverse(false), m_bPause(false), m_bIsPlaying(false)
{
}

CAnimController::~CAnimController()
{
}

void CAnimController::Destroy()
{
	m_pCurAnimation = nullptr;
	m_iFrameIndex = 0;
}

glm::mat4x4 CAnimController::GetMatrix()
{
	if (nullptr != m_pCurAnimation)
		return m_pCurAnimation->GetMatrix(m_iFrameIndex);

	return mat4x4(1.f);
}

void CAnimController::FrameMove(const _float& dt)
{
	if (m_bPause)
		return;

	if (nullptr != m_pCurAnimation)
	{
		m_fCurTime += dt * m_fAnimSpeed;
		m_pCurAnimation->FrameMove(m_fCurTime, m_iFrameIndex, m_bReverse
			, bind(&CAnimController::AnimationEndEvent, this));
	}
}

void CAnimController::SetNextFrame()
{
	if (m_iFrameIndex == m_pCurAnimation->GetAnimationLength() - 1)
		m_iFrameIndex = 0;
	else
		m_iFrameIndex++;

	m_fCurTime = 0.f;
}

void CAnimController::SetPreviousFrame()
{
	if (m_iFrameIndex == 0)
		m_iFrameIndex = m_pCurAnimation->GetAnimationLength() - 1;
	else
		m_iFrameIndex--;

	m_fCurTime = 0.f;
}

void CAnimController::SetReverse()
{
	m_bReverse = !m_bReverse;
}

void CAnimController::SetPause()
{
	m_bPause = !m_bPause;
}

void CAnimController::StopAnimation()
{

}

void CAnimController::ResetAnimation()
{
	m_fCurTime = 0;
	m_iFrameIndex = 0;
}
