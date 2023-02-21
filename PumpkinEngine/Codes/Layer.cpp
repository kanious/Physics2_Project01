#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Layer.h"
#include "..\Headers\GameObject.h"


USING(Engine)
USING(std)
_bool DeadFinder(CGameObject* pObj);

CLayer::CLayer()
{
	m_tag = 0;
	m_bEnable = true;
	m_listGameObjects.clear();
}

CLayer::~CLayer()
{
}

// Basic Update Function, update all Gameobjects that are stored in this layer
void CLayer::Update(const _float& dt)
{
	if (!m_bEnable)
		return;

	m_listGameObjects.erase(
		std::remove_if(m_listGameObjects.begin(), m_listGameObjects.end(), DeadFinder),
		m_listGameObjects.end());

	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (nullptr != (*iter))
			(*iter)->Update(dt);
	}
}

// Set this layer enabled
void CLayer::Enable()
{
	m_bEnable = true;
}

// Set this layer disabled
void CLayer::Disable()
{
	m_bEnable = false;
}

// Call instead of destructor to manage class internal data
void CLayer::Destroy()
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
		SafeDestroy(*iter);

	m_listGameObjects.clear();
}

// Set layer tag
void CLayer::SetTag(_uint layerTag)
{
	m_tag = layerTag;
}

// Add Gameobject to this layer
RESULT CLayer::AddGameObject(CGameObject* obj)
{
	if (nullptr == obj)
		return PK_GAMEOBJECT_NULLPTR;

	m_listGameObjects.push_back(obj);

	return PK_NOERROR;
}

// Remove Gameobject from this layer
RESULT CLayer::RemoveGameObject(CGameObject* obj)
{
	if (nullptr == obj)
		return PK_GAMEOBJECT_NULLPTR;

	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (nullptr != (*iter))
		{
			if ((*iter) == obj)
			{
				SafeDestroy(*iter);
				m_listGameObjects.remove(*iter);
				return PK_NOERROR;
			}
		}
	}

	return PK_GAMEOBJECT_CANNOT_FIND;
}

// Remove all Gameobject from this layer
void CLayer::RemoveAllGameObject()
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
		SafeDestroy(*iter);

	m_listGameObjects.clear();
}

// Find a Gameobject by object name
CGameObject* CLayer::FindGameObjectByObjName(string objName)
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (objName == (*iter)->GetObjName())
			return (*iter);
	}

	return nullptr;
}

// Initialize this layer
RESULT CLayer::Ready(_uint tag)
{
	m_tag = tag;

	return PK_NOERROR;
}

// Create this layer
CLayer* CLayer::Create(_uint tag)
{
	CLayer* pInstance = new CLayer;

	if (PK_NOERROR != pInstance->Ready(tag))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

// Find a dead Gameobject and safe delete it
_bool DeadFinder(CGameObject* pObj)
{
	_bool isDead = pObj->GetDead();

	if (isDead)
		SafeDestroy(pObj);

	return isDead;
}