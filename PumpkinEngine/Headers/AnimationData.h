#ifndef _ANIMATIONDATA_H_
#define _ANIMATIONDATA_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CAnimation;

//Class that oversees animation class
class ENGINE_API CAnimationData : public CBase
{
	SINGLETON(CAnimationData)

private:
	typedef std::unordered_map<std::string, CAnimation*> ANIM_MAP;
	ANIM_MAP				m_mapAnimations;

private:
	explicit CAnimationData();
	virtual ~CAnimationData();
	void Destroy();

public:
	void AddAnimation(std::string tag, CAnimation* pAnimation);
	CAnimation* FindAnimation(std::string tag);
	void LoadAnimations(std::string assetFolderPath);
};

NAMESPACE_END

#endif //_ANIMATIONDATA_H_