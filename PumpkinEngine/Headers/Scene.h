#ifndef _SCENE_H_
#define _SCENE_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CLayer;
class CGameObject;

//A scene class that holds game objects.
class ENGINE_API CScene : public CBase
{
protected:
	_uint							m_tag;
	std::vector<CLayer*>			m_vecLayer;

protected:
	std::string						m_DataPath;
	std::string						m_ObjListFileName;
	std::string						m_LightListFileName;

protected:
	explicit CScene();
	virtual ~CScene();

public:
	// Basic Update Function, update all Layers that are stored in this scene
	virtual void Update(const _float& dt);
	// Basic Render Function, use this if extra instance needs to be rendered
	virtual void Render();
protected:
	// Call instead of destructor to manage class internal data
	virtual void Destroy();

public:
	_uint GetSceneTag()								{ return m_tag; }
	std::string GetDataPath()						{ return m_DataPath; }
	std::string GetObjListFileName()				{ return m_ObjListFileName; }
	std::string GetLightListFileName()				{ return m_LightListFileName; }
	std::vector<CLayer*>& GetLayers()				{ return m_vecLayer; }

	void SetSceneTag(_uint sceneTag);
	void SetDataPath(std::string path)				{ m_DataPath = path; }
	void SetObjListFileName(std::string name)		{ m_ObjListFileName = name; }
	void SetLightListFileName(std::string name)		{ m_LightListFileName = name; }

	// Turn on/off the layer
	RESULT ToggleLayer(_uint layerTag, _bool turn);
	// Find layer by tag
	CLayer* GetLayer(_uint layerTag);
	// Add new layer
	RESULT AddLayer(_uint layerTag);
	// Add gameobject to layer
	RESULT AddGameObjectToLayer(_uint layerTag, CGameObject* pInstance);
};

NAMESPACE_END

#endif //_SCENE_H_