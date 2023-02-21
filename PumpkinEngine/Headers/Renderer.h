#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CGameObject;

// Only game objects registered here are rendered
class ENGINE_API CRenderer : public CBase
{
	SINGLETON(CRenderer)

private:
	std::vector<CGameObject*>		m_vecRenderObj;
	std::vector<CGameObject*>		m_vecTRenderObj;


private:
	explicit CRenderer();
	virtual ~CRenderer();
	void Destroy();
public:
	// Basic Render Function, translucent objects are rendered later than other objects
	void Render();

public:
	// Register objects that need to be rendered
	void AddRenderObj(CGameObject* pInstance, _bool isTransparent = false);
private:
	// Empty container
	void ClearAllRenderObjList();
};

NAMESPACE_END

#endif //_RENDERER_H_