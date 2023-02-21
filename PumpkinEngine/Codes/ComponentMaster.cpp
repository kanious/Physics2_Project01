#include "pch.h"
#include "..\Headers\ComponentMaster.h"
#include "..\Headers\Component.h"


USING(Engine)
USING(std)
SINGLETON_FUNCTION(CComponentMaster)

CComponentMaster::CComponentMaster()
{
	m_mapComponent.clear();
	m_vecMesh.clear();
}

CComponentMaster::~CComponentMaster()
{
}

// Call instead of destructor to manage class internal data
void CComponentMaster::Destroy()
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); ++iter)
		SafeDestroy(iter->second);

	m_mapComponent.clear();
	m_vecMesh.clear();
}

// Save new component
RESULT CComponentMaster::AddNewComponent(string tag, CComponent* pComponent)
{
	COMPONENT_MAP::iterator iter = m_mapComponent.find(tag);
	if (iter == m_mapComponent.end())
		m_mapComponent.insert(COMPONENT_MAP::value_type(tag, pComponent));
	else
		return PK_COMPONENT_EXIST;

	return PK_NOERROR;
}

// Save new mesh info (for map editor)
void CComponentMaster::AddNewMeshInfo(string tag)
{
	m_vecMesh.push_back(tag);
}

// Find component by tag
CComponent* CComponentMaster::FindComponent(std::string tag)
{
	CComponent* pComponent = nullptr;

	COMPONENT_MAP::iterator iter = m_mapComponent.find(tag);
	if (iter != m_mapComponent.end())
		pComponent = iter->second;

	return pComponent;
}
