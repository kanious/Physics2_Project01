#include "pch.h"
#include "..\Headers\Octree.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\CollisionMaster.h"
#include "..\Headers\BoundingBox.h"
#include "..\Headers\Transform.h"


USING(Engine)
USING(glm)
USING(std)

COctree::COctree()
	: m_pParentTransform(nullptr), m_bDebug(false)
{
	m_vecNodeInfo.clear();
}

COctree::~COctree()
{
}

// Call instead of destructor to manage class internal data
void COctree::Destroy()
{
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
void COctree::Render()
{
	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		if (m_vecNodeInfo[i]->vecTriangles.size() <= 0)
			continue;
		
		if (m_vecNodeInfo[i]->BBox_Render)
		{
			m_vecNodeInfo[i]->BBox->SetColor(vec3(1.f, 0.f, 0.f));
			m_vecNodeInfo[i]->BBox->RenderWithoutParent();
		}
		else
		{
			if (m_bDebug)
			{
				m_vecNodeInfo[i]->BBox->SetColor(vec3(0.f, 1.f, 0.f));
				m_vecNodeInfo[i]->BBox->RenderWithoutParent();
			}
		}
	}
}

// Add triangles for collision checking
void COctree::AddTriangle(TRIANGLE t, _uint& count)
{
	_bool flag = false;
	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		if (!m_vecNodeInfo[i]->isLeaf)
			continue;

		if (CCollisionMaster::GetInstance()->IntersectTriangleToOBB(&t, m_vecNodeInfo[i]->BBox))
		{
			TRIANGLE* tri = new TRIANGLE();
			tri->p0 = t.p0;
			tri->p1 = t.p1;
			tri->p2 = t.p2;
			m_vecNodeInfo[i]->vecTriangles.push_back(tri);
			flag = true;
		}
	}

	if (!flag)
	{
		++count;
	}
}

void COctree::CheckBoundingBox(CBoundingBox* bbox, vector<COctreeNode*>& vecNode)
{
	if (nullptr == bbox)
		return;

	for (int i = 0; i < m_vecNodeInfo.size(); ++i)
	{
		m_vecNodeInfo[i]->BBox_Render = false;
	}
	
	CheckNodeBoundingBox(0, bbox, vecNode);
}

void COctree::CheckNodeBoundingBox(_int index, CBoundingBox* bbox, vector<COctreeNode*>& vecNode)
{
	COctreeNode* pNode = m_vecNodeInfo[index];
	CBoundingBox* nodeBBBox = pNode->BBox;

	if (pNode->isLeaf && pNode->vecTriangles.size() == 0)
		return;

	if (CCollisionMaster::GetInstance()->IntersectOBBToAABB(bbox, nodeBBBox))
	{
		if (pNode->isLeaf)
		{
			pNode->BBox_Render = true;
			vecNode.push_back(pNode);
		}
		else
		{
			CheckNodeBoundingBox(pNode->childIndex[0], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[1], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[2], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[3], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[4], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[5], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[6], bbox, vecNode);
			CheckNodeBoundingBox(pNode->childIndex[7], bbox, vecNode);
		}
	}
}

_bool Compare(COctree::COctreeNode* t1, COctree::COctreeNode* t2)
{
	return t1->myIndex < t2->myIndex;
}

// Initialize
RESULT COctree::Ready(vec3 vMax, vec3 vMin, _uint depth)
{
	_uint nodeCount = (pow(8, depth) - 1) / 7;
	m_vecNodeInfo.resize(nodeCount);

	vec3 vHalf = (vMax - vMin) / 2.f;
	ReadyOctree(0, vMin + vHalf, vHalf, 1, depth);
	sort(m_vecNodeInfo.begin(), m_vecNodeInfo.end(), Compare);

	return PK_NOERROR;
}

// Initialize Octree
void COctree::ReadyOctree(_uint index, glm::vec3 vCenter, vec3 vHalf, _uint depth, _uint maxDepth)
{
	COctreeNode* pNode = new COctreeNode();
	pNode->myIndex = index;
	pNode->vMin = vCenter - vHalf;
	pNode->vMax = vCenter + vHalf;
	pNode->vHalf = vHalf;
	pNode->vCenter = vCenter;
	pNode->BBox = CBoundingBox::Create(pNode->vMin, pNode->vMax, "DebugBoxShader");
	pNode->BBox_Render = false;

	if (depth == maxDepth)
	{
		pNode->isLeaf = true;
	}
	else
	{
		pNode->isLeaf = false;

		pNode->childIndex[0] = 1 + (8 * index);
		pNode->childIndex[1] = 2 + (8 * index);
		pNode->childIndex[2] = 3 + (8 * index);
		pNode->childIndex[3] = 4 + (8 * index);
		pNode->childIndex[4] = 5 + (8 * index);
		pNode->childIndex[5] = 6 + (8 * index);
		pNode->childIndex[6] = 7 + (8 * index);
		pNode->childIndex[7] = 8 + (8 * index);

		vec3 childHalf = vHalf / 2.f;
		vec3 childCenter = vec3(0.f);
		childCenter.x = pNode->vCenter.x - childHalf.x;
		childCenter.y = pNode->vCenter.y - childHalf.y;
		childCenter.z = pNode->vCenter.z + childHalf.z;
		ReadyOctree(pNode->childIndex[0], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x + childHalf.x;
		childCenter.y = pNode->vCenter.y - childHalf.y;
		childCenter.z = pNode->vCenter.z + childHalf.z;
		ReadyOctree(pNode->childIndex[1], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x - childHalf.x;
		childCenter.y = pNode->vCenter.y - childHalf.y;
		childCenter.z = pNode->vCenter.z - childHalf.z;
		ReadyOctree(pNode->childIndex[2], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x + childHalf.x;
		childCenter.y = pNode->vCenter.y - childHalf.y;
		childCenter.z = pNode->vCenter.z - childHalf.z;
		ReadyOctree(pNode->childIndex[3], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x - childHalf.x;
		childCenter.y = pNode->vCenter.y + childHalf.y;
		childCenter.z = pNode->vCenter.z + childHalf.z;
		ReadyOctree(pNode->childIndex[4], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x + childHalf.x;
		childCenter.y = pNode->vCenter.y + childHalf.y;
		childCenter.z = pNode->vCenter.z + childHalf.z;
		ReadyOctree(pNode->childIndex[5], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x - childHalf.x;
		childCenter.y = pNode->vCenter.y + childHalf.y;
		childCenter.z = pNode->vCenter.z - childHalf.z;
		ReadyOctree(pNode->childIndex[6], childCenter, childHalf, depth + 1, maxDepth);

		childCenter.x = pNode->vCenter.x + childHalf.x;
		childCenter.y = pNode->vCenter.y + childHalf.y;
		childCenter.z = pNode->vCenter.z - childHalf.z;
		ReadyOctree(pNode->childIndex[7], childCenter, childHalf, depth + 1, maxDepth);
	}

	m_vecNodeInfo[pNode->myIndex] = pNode;
}

// Create an instance
COctree* COctree::Create(vec3 vMax, vec3 vMin, _uint depth)
{
	COctree* pInstance = new COctree();
	if (PK_NOERROR != pInstance->Ready(vMax, vMin, depth))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
