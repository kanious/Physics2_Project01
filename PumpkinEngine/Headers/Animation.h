#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>
#include "Base.h"
#include "glm\mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

// Class containing animation data
class ENGINE_API CAnimation : public CBase
{
private:
	std::string						m_tag;

	std::vector<glm::mat4x4>		m_vecMatrix;
	_uint							m_iDuration;
	_float							m_fTickPerSec;


private:
	explicit CAnimation();
	virtual ~CAnimation();

private:
	virtual void Destroy();

public:
	void FrameMove(_float& curTime, _uint& frameIdx, _bool reverse, std::function<void(void)> callback);
	glm::mat4x4 GetMatrix(_uint iFrameIndex);
	_uint GetAnimationLength()			{ return m_iDuration; }

private:
	RESULT Ready(std::string tag, std::string filePath);
public:
	static CAnimation* Create(std::string tag, std::string filePath);
};

NAMESPACE_END

#endif //_ANIMATION_H_