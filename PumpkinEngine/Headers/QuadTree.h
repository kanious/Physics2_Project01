#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Base.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

class CBoundingBox;

// The QuadTree Class
class ENGINE_API CQuadTree : public CBase
{
public:
	class CQuadTreeNode
	{
	public:
		_bool					isLeaf;
		glm::vec3				vMin;
		glm::vec3				vMax;
		glm::vec3				vCenter;
		glm::vec3				vHalf;
		_float					fRadius;
		_uint					myIndex;
		_uint					childIndex[4];
		std::vector<TRIANGLE*>	vecTriangles;
		CBoundingBox*			BBox;
		_bool					BBox_Render;
		explicit CQuadTreeNode()
			: isLeaf(false), myIndex(0), fRadius(0.f), BBox(nullptr), BBox_Render(false)
			, vMin(glm::vec3(0.f)), vMax(glm::vec3(0.f)), vCenter(glm::vec3(0.f)), vHalf(glm::vec3(0.f))
		{
			vecTriangles.clear();
			memset(childIndex, 0, sizeof(_uint) * 4);
		}
		~CQuadTreeNode() {}
	};

private:
	std::vector<CQuadTreeNode*>							m_vecNodeInfo;
	glm::vec3											m_vHashRange;
	typedef std::unordered_map<_int, CQuadTreeNode*>	MAP_NODE;
	MAP_NODE											m_mapNodes;

private:
	explicit CQuadTree();
	virtual ~CQuadTree();
	void Destroy();
public:
	void Render();

public:
	std::vector<CQuadTreeNode*> GetNodeVector()		{ return m_vecNodeInfo; }
	_int GetHashValue(_float x, _float z);
	void AddTriangleToTreeNode(_int hashKey, TRIANGLE triangle);
	void GetLeafNodes(glm::vec3 vMain, glm::vec3 vTarget, std::vector<CQuadTreeNode*>& vecNode);

private:
	void CheckDistanceQuadTree(_int index, glm::vec3 vCenter, _float fRadius, glm::vec3 vMain, glm::vec3 vDir, std::vector<CQuadTreeNode*>& vecNode);

private:
	RESULT Ready(glm::vec3 vMax, glm::vec3 vMin, _uint depth);
	void ReadyQuadTree(_uint index, glm::vec3 vMax, glm::vec3 vMin, _uint depth, _uint maxDepth);
public:
	static CQuadTree* Create(glm::vec3 vMax, glm::vec3 vMin, _uint depth);
};

NAMESPACE_END

#endif //_QUADTREE_H_