#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "Base.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

class CBoundingBox;
class CTransform;

// The Octree Class
class ENGINE_API COctree : public CBase
{
public:
	class COctreeNode
	{
	public:
		_bool					isLeaf;
		glm::vec3				vMin;
		glm::vec3				vMax;
		glm::vec3				vCenter;
		glm::vec3				vHalf;
		_uint					myIndex;
		_uint					childIndex[8];
		std::vector<TRIANGLE*>	vecTriangles;
		CBoundingBox*			BBox;
		_bool					BBox_Render;
		explicit COctreeNode()
			: isLeaf(false), myIndex(0), BBox(nullptr), BBox_Render(false)
			, vMin(glm::vec3(0.f)), vMax(glm::vec3(0.f)), vCenter(glm::vec3(0.f)), vHalf(glm::vec3(0.f))
		{
			vecTriangles.clear();
			memset(childIndex, 0, sizeof(_uint) * 4);
		}
		~COctreeNode() {}
	};

private:
	std::vector<COctreeNode*>			m_vecNodeInfo;
	CTransform*							m_pParentTransform;
	_bool								m_bDebug;

private:
	explicit COctree();
	virtual ~COctree();
	void Destroy();
public:
	void Render();

public:
	void SetParentTransform(CTransform* parent)		{ m_pParentTransform = parent; }
	void SetDebug(_bool value)						{ m_bDebug = value; }
	std::vector<COctreeNode*> GetNodeVector()		{ return m_vecNodeInfo; }
	_bool GetDebug()							{ return m_bDebug; }
	void AddTriangle(TRIANGLE t, _uint& count);
	void CheckBoundingBox(CBoundingBox* bbox, std::vector<COctreeNode*>& vecNode);
private:
	void CheckNodeBoundingBox(_int index, CBoundingBox* bbox, std::vector<COctreeNode*>& vecNode);

private:
	RESULT Ready(glm::vec3 vMax, glm::vec3 vMin, _uint depth);
	void ReadyOctree(_uint index, glm::vec3 vCenter, glm::vec3 vHalf, _uint depth, _uint maxDepth);
public:
	static COctree* Create(glm::vec3 vMax, glm::vec3 vMin, _uint depth);
};

NAMESPACE_END

#endif //_OCTREE_H_