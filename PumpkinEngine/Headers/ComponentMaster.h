#ifndef _COMPONENTMASTER_H_
#define _COMPONENTMASTER_H_

#include "Base.h"
#include "Component.h"

NAMESPACE_BEGIN(Engine)

class CComponent;
// A class that manages components. All components are cloned from here
class ENGINE_API CComponentMaster : public CBase
{
	SINGLETON(CComponentMaster)

private:
	typedef std::unordered_map<std::string, CComponent*> COMPONENT_MAP;
	COMPONENT_MAP				m_mapComponent;

	typedef std::vector<std::string> MESH_VECTOR;
	MESH_VECTOR					m_vecMesh;

private:
	explicit CComponentMaster();
	virtual ~CComponentMaster();
private:
	// Call instead of destructor to manage class internal data
	void Destroy();

public:
	// Save new component
	RESULT AddNewComponent(std::string tag, CComponent* pComponent);
	// Save new mesh info (for map editor)
	void AddNewMeshInfo(std::string tag);
	// Find component by tag
	CComponent* FindComponent(std::string tag);
	COMPONENT_MAP* GetComponentMap()			{ return &m_mapComponent; }
	MESH_VECTOR* GetMeshVector()				{ return &m_vecMesh; }
};

// clone component by tag
template <class T>
T CloneComponent(std::string tag)
{
	CComponent* pComponent = CComponentMaster::GetInstance()->FindComponent(tag);
	if (nullptr != pComponent)
	{
		CComponent* clone = pComponent->Clone();
		return dynamic_cast<T>(clone);
	}
	return nullptr;
}

NAMESPACE_END

#endif //_COMPONENTMASTER_H_