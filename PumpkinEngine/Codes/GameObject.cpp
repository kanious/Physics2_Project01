#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Component.h"
#include "..\Headers\GameObject.h"
#include "..\Headers\Renderer.h"
#include "..\Headers\Layer.h"
#include "..\Headers\Transform.h"
#include "..\Headers\ComponentMaster.h"
#include <Rpc.h> // For.UUID


USING(Engine)
USING(glm)
USING(std)

CGameObject::CGameObject()
{
	m_UUID = UUIDGenerate();
	m_meshName = "";
	m_objName = "";
	m_objTag = 0;
	m_layerTag = 0;
	m_sceneTag = 0;
	m_bEnable = true;
	m_bLock = false;
	m_bDebug = true;
	m_bWireFrame = false;
	m_bSelected = false;
	m_mapComponent.clear();
	m_pRenderer = CRenderer::GetInstance(); m_pRenderer->AddRefCnt();
	m_pLayer = nullptr;
	m_pBoundingBox = nullptr;

	m_pTransform = CloneComponent<CTransform*>("Transform");
	AttachComponent("Transform", m_pTransform);
}

CGameObject::~CGameObject()
{
}

// Basic Update Function, update all components that are stored in this Gameobject
void CGameObject::Update(const _float& dt)
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); iter++)
	{
		if (nullptr != iter->second)
		{
			if (iter->second->GetEnable())
				iter->second->Update(dt);
		}
	}
}

// Basic Render Function, render all components that are stored in this Gameobject
void CGameObject::Render()
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); ++iter)
	{
		if (nullptr != iter->second)
		{
			if (iter->second->GetEnable())
				iter->second->Render();
		}
	}
}

// Set this object enabled
void CGameObject::Enable()
{
	m_bEnable = true;
}

// Set this object disabled
void CGameObject::Disable()
{
	m_bEnable = false;
}

// Call instead of destructor to manage class internal data
void CGameObject::Destroy()
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); ++iter)
		SafeDestroy(iter->second);

	m_mapComponent.clear();

	SafeDestroy(m_pRenderer);

	::RpcStringFreeW((RPC_WSTR*)&m_UUID);
	m_UUID = NULL;
}

// Set tag information
void CGameObject::SetupGameObject(_uint sceneTag, _uint layerTag, _uint objTag)
{
	SetSceneTag(sceneTag);
	SetLayerTag(layerTag);
	SetObjectTag(objTag);
}

// Get position from the transform component
vec3 CGameObject::GetPosition()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetPosition();
}

// Get rotation from the transform component
vec3 CGameObject::GetRotation()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetRotation();
}

// Get rotationY from the transform component
_float CGameObject::GetRotationY()
{
	if (nullptr == m_pTransform)
		return 0.f;

	return m_pTransform->GetRotationY();
}

// Get scale from the transform component
vec3 CGameObject::GetScale()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetScale();
}

// Get normalized look vector from the transform component
glm::vec3 CGameObject::GetLookVector()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetLookVector();
}

glm::vec3 CGameObject::GetRightVector()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetRightVector();
}

// Get world matrix from the transform component
const mat4x4* CGameObject::GetWorldMatrix()
{
	if (nullptr == m_pTransform)
		return nullptr;

	return m_pTransform->GetWorldMatrix();
}

// Set mesh name
void CGameObject::SetMeshName(string name)
{
	m_meshName = name;
}

// Set object name
void CGameObject::SetObjectName(std::string name)
{
	m_objName = name;
}

// Set object tag (enum from client)
void CGameObject::SetObjectTag(_uint objTag)
{
	m_objTag = objTag;
}

// Set layer tag (enum from client)
void CGameObject::SetLayerTag(_uint layerTag)
{
	m_layerTag = layerTag;
}

// Set scene tag (enum from client)
void CGameObject::SetSceneTag(_uint sceneTag)
{
	m_sceneTag = sceneTag;
}

// Set parent layer where this object is stored
void CGameObject::SetParentLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}

// Set this object dead
void CGameObject::SetDead(_bool dead)
{
	m_bDead = dead;
}

// Set position to the transform component
void CGameObject::SetPosition(vec3 vPos)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetPosition(vPos);
}

// Set rotation to the transform component
void CGameObject::SetRotation(vec3 vRot)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotation(vRot);
}

// Set rotationX to the transform component
void CGameObject::SetRotationX(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationX(fAngle);
}

// Set rotationY to the transform component
void CGameObject::SetRotationY(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationY(fAngle);
}

// Set rotationZ to the transform component
void CGameObject::SetRotationZ(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationZ(fAngle);
}

// Set scale to the transform component
void CGameObject::SetScale(glm::vec3 vScale)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetScale(vScale);
}

// Set parent transform from parent object to the transform component
void CGameObject::SetParentTransform(CTransform* pTransform)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetParent(pTransform);
}

// Attach new component
void CGameObject::AttachComponent(std::string componentTag, CComponent* pInstance)
{
	if (nullptr == pInstance)
		return;

	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter != m_mapComponent.end())
	{
		SafeDestroy(iter->second);
		m_mapComponent.erase(iter);
	}
	m_mapComponent.insert(COMPONENT_MAP::value_type(componentTag, pInstance));
}

// Remove new component
RESULT CGameObject::RemoveComponent(std::string componentTag)
{
	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter == m_mapComponent.end())
		return PK_COMPONENT_CANNOT_FIND;
	else
	{
		SafeDestroy(iter->second);
		m_mapComponent.erase(iter);
	}

	return PK_NOERROR;
}

// Find Component by tag
CComponent* CGameObject::GetComponent(std::string componentTag)
{
	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter == m_mapComponent.end())
		return nullptr;
	else
		return iter->second;
}

// Generate unique UUID for this object
_wchar_t* CGameObject::UUIDGenerate()
{
	UUID uuid;
	RPC_STATUS ret_val = ::I_UuidCreate(&uuid);

	_wchar_t* wUuid = NULL;
	if (ret_val == RPC_S_OK)
		::UuidToStringW(&uuid, (RPC_WSTR*)&wUuid);
	return wUuid;
}