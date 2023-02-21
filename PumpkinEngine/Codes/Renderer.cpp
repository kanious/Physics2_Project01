#include "pch.h"
#include "..\Headers\Renderer.h"
#include "..\Headers\GameObject.h"


USING(Engine)
USING(std)
SINGLETON_FUNCTION(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Destroy()
{
	ClearAllRenderObjList();
}

// Basic Render Function, translucent objects are rendered later than other objects
void CRenderer::Render()
{
	vector<CGameObject*>::iterator iter;
	for (iter = m_vecRenderObj.begin(); iter != m_vecRenderObj.end(); ++iter)
	{
		if (nullptr != *iter)
			(*iter)->Render();
	}
	for (iter = m_vecTRenderObj.begin(); iter != m_vecTRenderObj.end(); ++iter)
	{
		if (nullptr != *iter)
			(*iter)->Render();
	}
	ClearAllRenderObjList();
}

// Register objects that need to be rendered
void CRenderer::AddRenderObj(CGameObject* pInstance, _bool isTransparent)
{
	if (nullptr == pInstance)
		return;

	if (!isTransparent)
		m_vecRenderObj.push_back(pInstance);
	else
		m_vecTRenderObj.push_back(pInstance);
}

// Empty container
void CRenderer::ClearAllRenderObjList()
{
	m_vecRenderObj.clear();
	m_vecTRenderObj.clear();
}
