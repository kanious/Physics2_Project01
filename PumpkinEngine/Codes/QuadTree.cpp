#include "pch.h"
#include "..\Headers\QuadTree.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\CollisionMaster.h"
#include "..\Headers\BoundingBox.h"


USING(Engine)
USING(glm)
USING(std)

CQuadTree::CQuadTree()
	: m_vHashRange(vec3(0.f))
{
	m_vecNodeInfo.clear();
	m_mapNodes.clear();
}

CQuadTree::~CQuadTree()
{
}

// Call instead of destructor to manage class internal data
void CQuadTree::Destroy()
{
	m_mapNodes.clear();
	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		for (int j = 0; j < m_vecNodeInfo[i]->vecTriangles.size(); ++j)
		{
			delete m_vecNodeInfo[i]->vecTriangles[j];
		}
		m_vecNodeInfo[i]->vecTriangles.clear();
		SafeDestroy(m_vecNodeInfo[i]->BBox);
		delete m_vecNodeInfo[i];
	}
	m_vecNodeInfo.clear();
}

// Basic Render Function, render bounding box of node
void CQuadTree::Render()
{
	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		if (m_vecNodeInfo[i]->BBox_Render)
			m_vecNodeInfo[i]->BBox->RenderWithoutParent();
	}
}

_int CQuadTree::GetHashValue(_float x, _float z)
{
	_float xValue = floor(x);
	xValue = floor(xValue + m_vHashRange.x);
	xValue = floor(xValue / m_vHashRange.x);
	xValue *= 10000;

	_float zValue = floor(z);
	zValue = floor(zValue + m_vHashRange.z);
	zValue = floor(zValue / m_vHashRange.z);

	return (_int)xValue + (_int)zValue;
}

// Add triangles for collision checking
void CQuadTree::AddTriangleToTreeNode(_int hashKey, TRIANGLE triangle)
{
	MAP_NODE::iterator iter = m_mapNodes.find(hashKey);
	if (iter != m_mapNodes.end())
	{
		TRIANGLE* newTriangle = new TRIANGLE();
		newTriangle->p0 = triangle.p0;
		newTriangle->p1 = triangle.p1;
		newTriangle->p2 = triangle.p2;
		iter->second->vecTriangles.push_back(newTriangle);
	}
}

void CQuadTree::GetLeafNodes(vec3 vMain, vec3 vTarget, vector<CQuadTreeNode*>& vecNode)
{
	vec3 vMax(0.f);
	vec3 vMin(0.f);

	if (vMain.x > vTarget.x) { vMax.x = vMain.x; vMin.x = vTarget.x; }
	else					 { vMax.x = vTarget.x; vMin.x = vMain.x; }

	if (vMain.y > vTarget.y) { vMax.y = vMain.y; vMin.y = vTarget.y; }
	else					 { vMax.y = vTarget.y; vMin.y = vMain.y; }

	if (vMain.z > vTarget.z) { vMax.z = vMain.z; vMin.z = vTarget.z; }
	else					 { vMax.z = vTarget.z; vMin.z = vMain.z; }

	vec3 vHalf = (vMax - vMin) / 2.f;
	vec3 vCenter = vMin + vHalf;
	_float fRadius = distance(vCenter, vMax);

	vec3 vDir = vTarget - vMain;
	vDir = normalize(vDir);

	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		m_vecNodeInfo[i]->BBox_Render = false;
	}

	CheckDistanceQuadTree(0, vCenter, fRadius, vMain, vDir, vecNode);
}

void CQuadTree::CheckDistanceQuadTree(_int index, vec3 vCenter, _float fRadius
	, vec3 vMain, vec3 vDir, vector<CQuadTreeNode*>& vecNode)
{
	CQuadTreeNode* pNode = m_vecNodeInfo[index];
	_float fCenterDist = distance(vCenter, pNode->vCenter);

	if (fCenterDist <= fRadius + pNode->fRadius + 5.0f)
	{
		_bool check = CCollisionMaster::GetInstance()->IntersectRayToBoundingBox(pNode->BBox, nullptr, vMain, vDir);
		if (check)
		{
			if (pNode->isLeaf)
			{
 				pNode->BBox_Render = true;
				vecNode.push_back(pNode);
			}
			else
			{
				CheckDistanceQuadTree(pNode->childIndex[0], vCenter, fRadius, vMain, vDir, vecNode);
				CheckDistanceQuadTree(pNode->childIndex[1], vCenter, fRadius, vMain, vDir, vecNode);
				CheckDistanceQuadTree(pNode->childIndex[2], vCenter, fRadius, vMain, vDir, vecNode);
				CheckDistanceQuadTree(pNode->childIndex[3], vCenter, fRadius, vMain, vDir, vecNode);
			}
		}
	}

}

_bool Compare(CQuadTree::CQuadTreeNode* t1, CQuadTree::CQuadTreeNode* t2)
{
	return t1->myIndex < t2->myIndex;
}

// Initialize
RESULT CQuadTree::Ready(vec3 vMax, vec3 vMin, _uint depth)
{
	_uint nodeCount = (pow(4, depth) - 1) / 3;
	m_vecNodeInfo.resize(nodeCount);
	ReadyQuadTree(0, vMax, vMin, 1, depth);
	sort(m_vecNodeInfo.begin(), m_vecNodeInfo.end(), Compare);

	return PK_NOERROR;
}

// Initialize QuadTree
void CQuadTree::ReadyQuadTree(_uint index, glm::vec3 vMax, glm::vec3 vMin, _uint depth, _uint maxDepth)
{
	CQuadTreeNode* pNode = new CQuadTreeNode();
	pNode->myIndex = index;
	pNode->vMax = vMax;
	pNode->vMin = vMin;
	pNode->vHalf = (vMax - vMin) / 2.f;
	pNode->vCenter = vMin + pNode->vHalf;
	pNode->vHalf.y = 0.f;
	pNode->vCenter.y = 0.f;
	pNode->fRadius = distance(pNode->vCenter, pNode->vMax);
	pNode->BBox = CBoundingBox::Create(pNode->vMin, pNode->vMax, "DebugBoxShader");

	if (depth == maxDepth)
	{
		pNode->isLeaf = true;
		m_vHashRange = vMax - vMin;

		_int hashedValue = GetHashValue(pNode->vMax.x, pNode->vMax.z);
		m_mapNodes.insert(unordered_map<_int, CQuadTreeNode*>::value_type(hashedValue, pNode));

		hashedValue = GetHashValue(pNode->vMin.x, pNode->vMin.z);
		m_mapNodes.insert(unordered_map<_int, CQuadTreeNode*>::value_type(hashedValue, pNode));

		hashedValue = GetHashValue(pNode->vCenter.x, pNode->vCenter.z);
		m_mapNodes.insert(unordered_map<_int, CQuadTreeNode*>::value_type(hashedValue, pNode));
	}
	else
	{
		pNode->isLeaf = false;

		pNode->childIndex[0] = 1 + (4 * index);
		pNode->childIndex[1] = 2 + (4 * index);
		pNode->childIndex[2] = 3 + (4 * index);
		pNode->childIndex[3] = 4 + (4 * index);

		ReadyQuadTree(pNode->childIndex[0], vec3(vMax.x - pNode->vHalf.x, vMax.y, vMax.z - pNode->vHalf.z), vMin, depth + 1, maxDepth);
		ReadyQuadTree(pNode->childIndex[1], vec3(vMax.x, vMax.y, vMax.z - pNode->vHalf.z), vec3(vMin.x + pNode->vHalf.x, vMin.y, vMin.z), depth + 1, maxDepth);
		ReadyQuadTree(pNode->childIndex[2], vec3(vMax.x - pNode->vHalf.x, vMax.y, vMax.z), vec3(vMin.x, vMin.y, vMin.z + pNode->vHalf.z), depth + 1, maxDepth);
		ReadyQuadTree(pNode->childIndex[3], vMax, vec3(vMin.x + pNode->vHalf.x, vMin.y, vMin.z + pNode->vHalf.z), depth + 1, maxDepth);
	}

	m_vecNodeInfo[pNode->myIndex] = pNode;
}

// Create an instance
CQuadTree* CQuadTree::Create(vec3 vMax, vec3 vMin, _uint depth)
{
	CQuadTree* pInstance = new CQuadTree();
	if (PK_NOERROR != pInstance->Ready(vMax, vMin, depth))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
