#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Scene.h"
#include "..\Headers\Layer.h"
#include "..\Headers\GameObject.h"


USING(Engine)
USING(std)

CScene::CScene()
	: m_tag(0), m_DataPath(""), m_ObjListFileName(""), m_LightListFileName("")
{
	m_tag = 0;
	m_vecLayer.clear();
}

CScene::~CScene()
{
}

// Basic Update Function, update all Layers that are stored in this scene
void CScene::Update(const _float& dt)
{
	vector<CLayer*>::iterator iter;
 	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter)
			(*iter)->Update(dt);
	}
}

// Basic Render Function, use this if extra instance needs to be rendered
void CScene::Render()
{
}

//Call instead of destructor to manage class internal data
void CScene::Destroy()
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
		SafeDestroy(*iter);

	m_vecLayer.clear();
}

// Set tag
void CScene::SetSceneTag(_uint sceneTag)
{
	m_tag = sceneTag;
}

// Turn on/off the layer
RESULT CScene::ToggleLayer(_uint layerTag, _bool turn)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
		{
			if (turn)
				(*iter)->Enable();
			else
				(*iter)->Disable();

			return PK_NOERROR;
		}
	}

	return PK_LAYER_CANNOT_FIND;
}

// Find layer by tag
CLayer* CScene::GetLayer(_uint layerTag)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
				return (*iter);
	}

	CLayer* pLayer = CLayer::Create(layerTag);
	m_vecLayer.push_back(pLayer);

	return pLayer;
}

// Add new layer
RESULT CScene::AddLayer(_uint layerTag)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
			return PK_LAYER_EXIST;
	}

	CLayer* pLayer = CLayer::Create(layerTag);
	m_vecLayer.push_back(pLayer);

	return PK_NOERROR;
}

// Add gameobject to layer
RESULT CScene::AddGameObjectToLayer(_uint layerTag, CGameObject* pInstance)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
		{
			(*iter)->AddGameObject(pInstance);
			return PK_NOERROR;
		}
	}
	return PK_LAYER_CANNOT_FIND;
}