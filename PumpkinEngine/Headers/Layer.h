#ifndef _LAYER_H_
#define _LAYER_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CGameObject;
class CObjectManager;
class CScene;

//A container class that stores GameObjects separately in the scene.
class ENGINE_API CLayer : public CBase
{
protected:
	_uint							m_tag;
	_bool							m_bEnable;

	std::list<CGameObject*>			m_listGameObjects;

protected:
	explicit CLayer();
	virtual ~CLayer();

public:
	// Basic Update Function, update all Gameobjects that are stored in this layer
	virtual void Update(const _float& dt);
	// Set this layer enabled
	void Enable();
	// Set this layer disabled
	void Disable();
private:
	// Call instead of destructor to manage class internal data
	virtual void Destroy();

public:
	const _uint GetTag()						{ return m_tag; }
	const _bool GetEnable()						{ return m_bEnable; }
	std::list<CGameObject*>* GetObjectList()	{ return &m_listGameObjects; }
	// Set layer tag
	void SetTag(_uint layerTag);
	void SetEnable(_bool value)					{ m_bEnable = value; }
	// Add Gameobject to this layer
	RESULT AddGameObject(CGameObject* obj);
	// Remove Gameobject from this layer
	RESULT RemoveGameObject(CGameObject* obj);
	// Remove all Gameobject from this layer
	void RemoveAllGameObject();
	// Find a Gameobject by object name
	CGameObject* FindGameObjectByObjName(std::string objName);

private:
	RESULT Ready(_uint tag);
public:
	static CLayer* Create(_uint tag);
};

NAMESPACE_END

#endif //_LAYER_H_