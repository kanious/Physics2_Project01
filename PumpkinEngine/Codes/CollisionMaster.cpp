#include "pch.h"
#include "..\Headers\CollisionMaster.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\BoundingBox.h"
#include "..\Headers\Transform.h"
#include "..\Headers\QuadTree.h"
#include "..\Headers\Octree.h"
#include "..\Headers\EngineStruct.h"
#include <vector>
#include <limits>


USING(Engine)
USING(glm)
USING(std)
SINGLETON_FUNCTION(CCollisionMaster)

CCollisionMaster::CCollisionMaster()
{
}

CCollisionMaster::~CCollisionMaster()
{
}

// Call instead of destructor to manage class internal data
void CCollisionMaster::Destroy()
{
}

_bool CCollisionMaster::IntersectRayToVirtualPlane(_float planeSize, vec3& vOrigin, vec3& vDir, vec3& vDest)
{
	vec3 p0 = vec3(-planeSize, 0.f, -planeSize);
	vec3 p1 = vec3(-planeSize, 0.f, planeSize);
	vec3 p2 = vec3(planeSize, 0.f, planeSize);
	vec3 p3 = vec3(planeSize, 0.f, -planeSize);

	if (IntersectPointToTriangle(p0, p1, p2, vOrigin, vDir, vDest))
		return true;

	if (IntersectPointToTriangle(p0, p2, p3, vOrigin, vDir, vDest))
		return true;

	return false;
}

_bool CCollisionMaster::IntersectRayToSphere(vec3& vOrigin, vec3& vDir, vec3 sCenter, _float sphereRadius)
{
	vec3 between = vOrigin - sCenter;

	_float p = dot(vDir, between);
	_float q = dot(between, between) - (sphereRadius * sphereRadius);

	float discriminant = (p * p) - q;
	if (discriminant < 0.f)
		return false;

	return true;
}

_bool CCollisionMaster::IntersectRayToBoundingBox(CBoundingBox* pBoundingBox, CTransform* pParentTransform, vec3& vOrigin, vec3& vDir)
{
	if (nullptr == pBoundingBox)
		return false;
	
	vec3 vParentPos = vec3(0.f);
	_float fAngleY = 0.f;
	vec3 vParentScale = vec3(1.f);

	if (nullptr != pParentTransform)
	{
		vParentPos = pParentTransform->GetPosition();
		fAngleY = pParentTransform->GetRotationY();
		vParentScale = pParentTransform->GetScale();
	}
	fAngleY = radians(fAngleY);

	VTX* pVertices = pBoundingBox->GetVertices();

	vec3 point[8];
	ZeroMemory(point, sizeof(point));
	for (int i = 0; i < 8; ++i)
	{
		vec3 vPos = vec3(pVertices[i].vPos.x, pVertices[i].vPos.y, pVertices[i].vPos.z);
		point[i] = vPos * vParentScale;
		_float x = point[i].x;
		_float z = point[i].z;
		point[i].x = (z * sin(fAngleY)) + (x * cos(fAngleY)); //x = zsin(b) + xcos(b);
		point[i].z = (z * cos(fAngleY)) - (x * sin(fAngleY)); //z = zcos(b) - xsin(b);
		point[i] = point[i] + vParentPos;
	}

	vec3 vDest;
	if (IntersectPointToTriangle(point[0], point[1], point[2], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[0], point[2], point[3], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[0], point[4], point[5], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[0], point[5], point[1], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[2], point[6], point[7], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[2], point[7], point[3], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[1], point[5], point[6], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[1], point[6], point[2], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[3], point[7], point[4], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[3], point[4], point[0], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[5], point[4], point[7], vOrigin, vDir, vDest))return true;
	if (IntersectPointToTriangle(point[5], point[7], point[6], vOrigin, vDir, vDest))return true;

	return false;
}

_bool CCollisionMaster::IntersectPointToTriangle(vec3& p0, vec3& p1, vec3& p2, vec3& vOrigin, vec3& vDir, vec3& vDest)
{
	vec3 vNormal = cross(p2 - p0, p1 - p0);
	_float n = dot(p0 - vOrigin, vNormal);
	_float d = n / dot(vDir, vNormal);
	vDest = vOrigin + vDir * d;

	return PointInTriangle(vDest, p0, p1, p2);
}

_bool CCollisionMaster::PointInTriangle(vec3 P, vec3 A, vec3 B, vec3 C)
{
	return (PointInDotThree(P, A, B, C) && PointInDotThree(P, B, C, A) && PointInDotThree(P, C, A, B))
		|| (PointInDotThree(P, A, C, B) && PointInDotThree(P, B, A, C) && PointInDotThree(P, C, B, A));
}

_bool CCollisionMaster::PointInDotThree(vec3 p1, vec3 p2, vec3 pA, vec3 pB)
{
	vec3 cp1 = cross(pB - pA, p1 - pA);
	vec3 cp2 = cross(pB - pA, p2 - pA);
	return dot(cp1, cp2) >= 0;
}


_bool CCollisionMaster::IntersectRayToTriangles(CQuadTree* pQuadTree, vec3& vMain, vec3& vTarget, vec3& vDest)
{
	if (nullptr == pQuadTree)
		return false;

	vec3 vDir = vTarget - vMain;
	vDir = normalize(vDir);

	vector<CQuadTree::CQuadTreeNode*> vecFullList = pQuadTree->GetNodeVector();
	for (int i = 0; i < vecFullList.size(); ++i)
	{
		vector<TRIANGLE*> vecTriangles = vecFullList[i]->vecTriangles;
		for (int j = 0; j < vecTriangles.size(); ++j)
		{
			vec3 p0 = vecTriangles[j]->p0;
			vec3 p1 = vecTriangles[j]->p1;
			vec3 p2 = vecTriangles[j]->p2;

			if (IntersectPointToTriangle(p0, p1, p2, vMain, vDir, vDest))
				return true;
		}
	}

	return false;
}

_bool CCollisionMaster::IsRayBlockedByTriangles(CQuadTree* pQuadTree, glm::vec3& vMain, glm::vec3& vTarget, glm::vec3& vDest)
{
	if (nullptr == pQuadTree)
		return false;

	vector<CQuadTree::CQuadTreeNode*> vecNode;
	pQuadTree->GetLeafNodes(vMain, vTarget, vecNode);

	_float maxX, maxY, maxZ, minX, minY, minZ;

	if (vMain.x > vTarget.x) { maxX = vMain.x; minX = vTarget.x; }
	else { maxX = vTarget.x; minX = vMain.x; }

	if (vMain.y > vTarget.y) { maxY = vMain.y; minY = vTarget.y; }
	else { maxY = vTarget.y; minY = vMain.y; }

	if (vMain.z > vTarget.z) { maxZ = vMain.z; minZ = vTarget.z; }
	else { maxZ = vTarget.z; minZ = vMain.z; }

	vec3 vDir = vTarget- vMain;
	vDir = normalize(vDir);
	_float fDist = distance(vTarget, vMain);

	for (int i = 0; i < vecNode.size(); ++i)
	{
		vector<TRIANGLE*> vecTriangles = vecNode[i]->vecTriangles;
		for (int j = 0; j < vecTriangles.size(); ++j)
		{
			vec3 p0 = vecTriangles[j]->p0;
			vec3 p1 = vecTriangles[j]->p1;
			vec3 p2 = vecTriangles[j]->p2;

			if ((p0.x < minX || p0.x > maxX) && (p0.y < minY || p0.y > maxY) && (p0.z < minZ || p0.z > maxZ) &&
				(p1.x < minX || p1.x > maxX) && (p1.y < minY || p1.y > maxY) && (p1.z < minZ || p1.z > maxZ) &&
				(p2.x < minX || p2.x > maxX) && (p2.y < minY || p2.y > maxY) && (p2.z < minZ || p2.z > maxZ))
			{
				continue;
			}

			vec3 vCenter(0.f);
			_float radius = 0.f;
			GetCenter(p0, p1, p2, vCenter, radius);
			if (fDist < distance(vCenter, vMain))
				continue;
			else if (IntersectRaySphere(vMain, vDir, vCenter, radius))
			{
				return true;
				//if (IntersectPointToTriangle(p0, p1, p2, vMain, vDir, vDest))
				//{
				//	return true;
				//}
			}
		}
	}

	return false;
}

_bool CCollisionMaster::IntersectCheckForProjectiles(CQuadTree* pQuadTree, glm::vec3& vMain, glm::vec3& vTarget, glm::vec3& vDest)
{
	if (nullptr == pQuadTree)
		return false;

	vector<CQuadTree::CQuadTreeNode*> vecNode;
	pQuadTree->GetLeafNodes(vMain, vTarget, vecNode);

	vec3 vDir = vTarget - vMain;
	vDir = normalize(vDir);
	_float fDist = distance(vTarget, vMain);

	for (int i = 0; i < vecNode.size(); ++i)
	{
		vector<TRIANGLE*> vecTriangles = vecNode[i]->vecTriangles;
		for (int j = 0; j < vecTriangles.size(); ++j)
		{
			vec3 p0 = vecTriangles[j]->p0;
			vec3 p1 = vecTriangles[j]->p1;
			vec3 p2 = vecTriangles[j]->p2;

			vec3 vCenter(0.f);
			_float radius = 0.f;
			GetCenter(p0, p1, p2, vCenter, radius);
			if (fDist < distance(vCenter, vMain))
				continue;
			else if (IntersectRaySphere(vMain, vDir, vCenter, radius))
			{
				if (IntersectPointToTriangle(p0, p1, p2, vMain, vDir, vDest))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CCollisionMaster::GetCenter(vec3& p0, vec3& p1, vec3& p2, vec3& center, _float& radius)
{
	center = vec3((p0.x + p1.x + p2.x) / 3, (p0.y + p1.y + p2.y) / 3, (p0.z + p1.z + p2.z) / 3);

	radius = distance(center, p0);
}

_bool CCollisionMaster::IntersectRaySphere(vec3& vOrigin, vec3& vDir, vec3& center, _float& radius)
{
	_float t0, t1;  //solutions for t if the ray intersects 
	_float radius2 = radius * radius;

	vec3 L = center - vOrigin;
	_float tca = dot(L, vDir);
	_float d2 = dot(L, L) - tca * tca;

	if (d2 > radius2)
		return false;

	_float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1)
		swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return false;
	}

	return true;
}

//_bool CCollisionMaster::TestTriangleAABB(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, CBoundingBox* aabb)
//{
//	_float p0, p1, r;
//
//	vec3 vMax = aabb->m_vMax;
//	vec3 vMin = aabb->m_vMin;
//	vec3 center = aabb->m_vCenter;
//	_float e0 = (vMax.x - vMin.x) * 0.5f;
//	_float e1 = (vMax.y - vMin.y) * 0.5f;
//	_float e2 = (vMax.z - vMin.z) * 0.5f;
//
//	// Translate triangle as conceptually moving AABB to origin
//	v0 = v0 - center;
//	v1 = v1 - center;
//	v2 = v2 - center;
//
//	// Compute edge vectors for triangle
//	vec3 f0 = v1 - v0;
//	vec3 f1 = v2 - v1;
//	vec3 f2 = v0 - v2;
//
//	// Test axes a00..a22 (category 3)
//	// a00
//	p0 = v0.z * v1.y - v0.y * v1.z;
//	p1 = v2.z * v1.y - v2.z * v0.y + v0.z * v2.y - v1.z * v2.y;
//	r = e1 * abs(f0.z) + e2 * abs(f0.y);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false; // axis is a separating axis
//	// a01
//	p0 = v0.y * v1.z - v0.y * v2.z - v0.z * v1.y + v0.z * v2.y;
//	p1 = -v1.y * v2.z + v1.z * v2.y;
//	r = e1 * abs(f1.z) + e2 * abs(f1.y);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//	// a02
//	p0 = v0.y * v2.z - v0.z * v2.y;
//	p1 = v0.y * v1.z - v0.z * v1.y + v1.y * v2.z - v1.z * v2.y;
//	r = e1 * abs(f2.z) + e2 * abs(f2.y);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//
//	// a10
//	p0 = v0.x * v1.z - v0.z * v1.x;
//	p1 = v0.x * v2.z - v0.z * v2.x - v1.x * v2.z + v1.z * v2.x;
//	r = e0 * abs(f0.z) + e2 * abs(f0.x);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//	// a11
//	p0 = -v0.x * v1.z + v0.x * v2.z + v0.z * v1.x - v0.z * v2.x;
//	p1 = v1.x * v2.z - v1.z * v2.x;
//	r = (e0 * abs(f1.z) + e2 * abs(f1.x));
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//	// a12
//	p0 = -v0.x * v2.z + v0.z * v2.x;
//	p1 = -v0.x * v1.z + v0.z * v1.x - v1.x * v2.z + v1.z * v2.x;
//	r = e0 * abs(f2.z) + e2 * abs(f2.x);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//
//	// a20
//	p0 = -v0.x * v1.y + v0.y * v1.x;
//	p1 = -v0.x * v2.y + v0.y * v2.x + v1.x * v2.y - v1.y * v2.x;
//	r = e0 * abs(f0.y) + e1 * abs(f0.x);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//	// a21
//	p0 = v0.x * v1.y - v0.x * v2.y - v0.y * v1.x + v0.y * v2.x;
//	p1 = -v1.x * v2.y + v1.y * v2.x;
//	r = e0 * abs(f1.y) + e1 * abs(f1.x);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//	// a22
//	p0 = v0.x * v2.y - v0.y * v2.x;
//	p1 = v0.x * v1.y - v0.y * v1.x + v1.x * v2.y - v1.y * v2.x;
//	r = e0 * abs(f2.y) + e1 * abs(f2.x);
//	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
//		return false;
//
//
//	// Test the three axes corresponding to the face normals of AABB b (category 1).
//	// ... [-e0, e0] and [min(v0.x, v1.x, v2.x), max(v0.x, v1.x, v2.x)] do not overlap
//	if (std::max(std::max(v0.x, v1.x), v2.x) < -e0 || std::min(std::min(v0.x, v1.x), v2.x) > e0) return false;
//	// ... [-e1, e1] and [min(v0.y, v1.y, v2.y), max(v0.y, v1.y, v2.y)] do not overlap
//	if (std::max(std::max(v0.y, v1.y), v2.y) < -e1 || std::min(std::min(v0.y, v1.y), v2.y) > e1) return false;
//	// ... [-e2, e2] and [min(v0.z, v1.z, v2.z), max(v0.z, v1.z, v2.z)] do not overlap
//	if (std::max(std::max(v0.z, v1.z), v2.z) < -e2 || std::min(std::min(v0.z, v1.z), v2.z) > e2) return false;
//
//
//	// Test separating axis corresponding to triangle face normal (category 2)
//	vec3 vNormal = cross(f0, f1);
//	_float dotValue = dot(vNormal, v0);
//
//	//r = e0 * abs(vNormal.x) + e1 * vNormal.y + e2 * vNormal.z;
//	//_float s = vNormal.x * center.x + vNormal.y * center.y + vNormal.z * center.z - dotValue;
//
//	//return abs(s) <= r;
//
//	//Plane p;
//	//p.n = Cross(f0, f1);
//	//p.d = Dot(p.n, v0);
//
//
//	//return TestAABBPlane();
//
//	//return _int();
//	return true;
//}
//
//_bool CCollisionMaster::IntersectTriangleInAABB(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, CBoundingBox* aabb)
//{
//	vec3 vMax = aabb->m_vMax;
//	vec3 vMin = aabb->m_vMin;
//
//	if (v0.x <= vMax.x && v0.x >= vMin.x && v0.y <= vMax.y && v0.y >= vMin.y && v0.z <= vMax.z && v0.z >= vMin.z)
//		return true;
//	if (v1.x <= vMax.x && v1.x >= vMin.x && v1.y <= vMax.y && v1.y >= vMin.y && v1.z <= vMax.z && v1.z >= vMin.z)
//		return true;
//	if (v2.x <= vMax.x && v2.x >= vMin.x && v2.y <= vMax.y && v2.y >= vMin.y && v2.z <= vMax.z && v2.z >= vMin.z)
//		return true;
//
//	return false;
//}
//
//_int CCollisionMaster::IntersectAABBToAABB(CBoundingBox* A, CBoundingBox* B)
//{
//	//if (A->m_vMax.x <= B->m_vMin.x)
//	//	return 1;
//
//	//if (A->m_vMin.x >= B->m_vMax.x)
//	//	return 2;
//
//	//if (A->m_vMax.y < B->m_vMin.y)
//	//	return 3;
//
//	//if (A->m_vMin.y > B->m_vMax.y)
//	//	return 4;
//
//	//if (A->m_vMax.z < B->m_vMin.z)
//	//	return 5;
//
//	//if (A->m_vMin.z > B->m_vMax.z)
//	//	return 6;
//
//	//return 0;
//
//
//	if (A->m_vMax.x < B->m_vMin.x || A->m_vMin.x > B->m_vMax.x)
//		return 0;
//
//	if (A->m_vMax.y < B->m_vMin.y || A->m_vMin.y > B->m_vMax.y)
//		return 0;
//
//	if (A->m_vMax.z < B->m_vMin.z || A->m_vMin.z > B->m_vMax.z)
//		return 0;
//
//	return 1;
//}
//
//int TestAABBPlane(CBoundingBox* b, vec3 vNormal, _float fDot)
//{
//	//// These two lines not necessary with a (Center, extents) AABB representation
//	//Point c = (b.max + b.min) * 0.5f; // Compute AABB center
//	//Point e = b.max - c; // Compute positive extents
//	//// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
//	//float r = e[0] * Abs(p.n[0]) + e[1] * Abs(p.n[1]) + e[2](Abs(p.n[2]);
//	//// Compute distance of box center from plane
//	//float s = Dot(p.n, c) - p.d;
//	//// Intersection occurs when distance s falls within [-r, +r] interval
//	//return Abs(s) <= r;
//
//	return 0;
//}


_bool CCollisionMaster::IntersectTriangleToAABB(TRIANGLE* triangle, CBoundingBox* boundingBox)
{
	_float tri_min, tri_max;

	vec3 bbMin = boundingBox->m_vMin;
	vec3 bbMax = boundingBox->m_vMax;

	vec3 vNormalX = vec3(1.f, 0.f, 0.f);
	vec3 vNormalY = vec3(0.f, 1.f, 0.f);
	vec3 vNormalZ = vec3(0.f, 0.f, 1.f);

	ProjectTriangle(triangle, vNormalX, tri_min, tri_max);
	if (tri_max < bbMin.x || tri_min > bbMax.x)
		return false;

	ProjectTriangle(triangle, vNormalY, tri_min, tri_max);
	if (tri_max < bbMin.y || tri_min > bbMax.y)
		return false;

	ProjectTriangle(triangle, vNormalZ, tri_min, tri_max);
	if (tri_max < bbMin.z || tri_min > bbMax.z)
		return false;

	_float box_min, box_max;
	vec3 triNormal = cross(triangle->p1 - triangle->p0, triangle->p2 - triangle->p0);
	_float triangleOffset = dot(triNormal, triangle->p0);
	ProjectBox(bbMin, bbMax, triNormal, box_min, box_max);
	if (box_max < triangleOffset || box_min > triangleOffset)
		return false;

	vec3 tri_edgesX = triangle->p0 - triangle->p1;
	vec3 tri_edgesY = triangle->p1 - triangle->p2;
	vec3 tri_edgesZ = triangle->p2 - triangle->p0;

	vec3 axis = cross(tri_edgesX, vNormalX);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesX, vNormalY);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesX, vNormalZ);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	axis = cross(tri_edgesY, vNormalX);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesY, vNormalY);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesY, vNormalZ);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	axis = cross(tri_edgesZ, vNormalX);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesZ, vNormalY);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesZ, vNormalZ);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	return true;
}

_bool CCollisionMaster::IntersectTriangleToOBB(TRIANGLE* triangle, CBoundingBox* boundingBox)
{
	_float tri_min, tri_max;
	_float box_min, box_max;

	vec3 bbMin = boundingBox->m_vMin;
	vec3 bbMax = boundingBox->m_vMax;

	vec3 vNormalLook = vec3(1.f, 0.f, 0.f);
	vec3 vNormalUp = vec3(0.f, 1.f, 0.f);
	vec3 vNormalRight = vec3(0.f, 0.f, 1.f);

	if (nullptr != boundingBox->m_pParentTransform)
	{
		vNormalLook = boundingBox->m_pParentTransform->GetLookVector();
		vNormalUp = boundingBox->m_pParentTransform->GetUpVector();
		vNormalRight = boundingBox->m_pParentTransform->GetRightVector();
	}

	ProjectBox(bbMin, bbMax, vNormalLook, box_min, box_max);
	ProjectTriangle(triangle, vNormalLook, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	ProjectBox(bbMin, bbMax, vNormalUp, box_min, box_max);
	ProjectTriangle(triangle, vNormalUp, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	ProjectBox(bbMin, bbMax, vNormalRight, box_min, box_max);
	ProjectTriangle(triangle, vNormalRight, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;


	vec3 triNormal = cross(triangle->p1 - triangle->p0, triangle->p2 - triangle->p0);
	_float triangleOffset = dot(triNormal, triangle->p0);
	ProjectBox(bbMin, bbMax, triNormal, box_min, box_max);
	if (box_max < triangleOffset || box_min > triangleOffset)
		return false;


	vec3 tri_edgesX = triangle->p0 - triangle->p1;
	vec3 tri_edgesY = triangle->p1 - triangle->p2;
	vec3 tri_edgesZ = triangle->p2 - triangle->p0;

	vec3 axis = cross(tri_edgesX, vNormalLook);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesX, vNormalUp);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesX, vNormalRight);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	axis = cross(tri_edgesY, vNormalLook);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesY, vNormalUp);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesY, vNormalRight);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	axis = cross(tri_edgesZ, vNormalLook);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesZ, vNormalUp);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;
	axis = cross(tri_edgesZ, vNormalRight);
	ProjectBox(bbMin, bbMax, axis, box_min, box_max);
	ProjectTriangle(triangle, axis, tri_min, tri_max);
	if (box_max < tri_min || box_min > tri_max)
		return false;

	return true;
}

_bool CCollisionMaster::IntersectOBBToAABB(CBoundingBox* obb, CBoundingBox* aabb)
{
	_float box1_min, box1_max;
	_float box2_min, box2_max;

	vec3 bbMin1 = obb->m_vMin;
	vec3 bbMax1 = obb->m_vMax;
	vec3 bbMin2 = aabb->m_vMin;
	vec3 bbMax2 = aabb->m_vMax;

	vec3 vRight1 = vec3(1.f, 0.f, 0.f);
	vec3 vUp1 = vec3(0.f, 1.f, 0.f);
	vec3 vLook1 = vec3(0.f, 0.f, 1.f);

	vec3 vRight2 = vec3(1.f, 0.f, 0.f);
	vec3 vUp2 = vec3(0.f, 1.f, 0.f);
	vec3 vLook2 = vec3(0.f, 0.f, 1.f);
	
	_bool fix = false;
	if (nullptr != obb->m_pParentTransform)
	{
		vLook1 = obb->m_pParentTransform->GetLookVector();
		vUp1 = obb->m_pParentTransform->GetUpVector();
		vRight1 = obb->m_pParentTransform->GetRightVector();

		_float angle = obb->m_pParentTransform->GetRotationY();
		if (angle < 90.f || (angle > 180.f && angle < 270.f))
			fix = true;
	}

	ProjectBox(bbMin1, bbMax1, vLook1, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, vLook1, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	ProjectBox(bbMin1, bbMax1, vUp1, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, vUp1, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	ProjectBox(bbMin1, bbMax1, vRight1, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, vRight1, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;

	ProjectBox(bbMin1, bbMax1, vLook2, box1_min, box1_max);
	if (bbMax2.z < box1_min || bbMin2.z > box1_max)
		return false;
	ProjectBox(bbMin1, bbMax1, vUp2, box1_min, box1_max);
	if (bbMax2.y < box1_min || bbMin2.y > box1_max)
		return false;
	ProjectBox(bbMin1, bbMax1, vRight2, box1_min, box1_max);
	if (bbMax2.x < box1_min || bbMin2.x > box1_max)
		return false;

	vec3 axis = cross(vLook1, vLook2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vLook1, vUp2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vLook1, vRight2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;

	axis = cross(vUp1, vLook2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vUp1, vUp2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vUp1, vRight2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;

	axis = cross(vRight1, vLook2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vRight1, vUp2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;
	axis = cross(vRight1, vRight2);
	ProjectBox(bbMin1, bbMax1, axis, box1_min, box1_max);
	ProjectBox(bbMin2, bbMax2, axis, box2_min, box2_max);
	if (box2_max < box1_min || box2_min > box1_max)
		return false;

	return true;
}

void CCollisionMaster::ProjectTriangle(TRIANGLE* triangle, vec3& axis, _float& fMin, _float& fMax)
{
	fMin = 100000.f;
	fMax = -100000.f;

	Project(axis, triangle->p0, fMin, fMax);
	Project(axis, triangle->p1, fMin, fMax);
	Project(axis, triangle->p2, fMin, fMax);
}

void CCollisionMaster::ProjectBox(vec3& bbMin, vec3& bbMax, vec3& axis, _float& fMin, _float& fMax)
{
	fMin = 100000.f;
	fMax = -100000.f;

	Project(axis, vec3(bbMin.x, bbMin.y, bbMin.z), fMin, fMax);
	Project(axis, vec3(bbMax.x, bbMin.y, bbMin.z), fMin, fMax);
	Project(axis, vec3(bbMin.x, bbMax.y, bbMin.z), fMin, fMax);
	Project(axis, vec3(bbMax.x, bbMax.y, bbMin.z), fMin, fMax);

	Project(axis, vec3(bbMin.x, bbMin.y, bbMax.z), fMin, fMax);
	Project(axis, vec3(bbMax.x, bbMin.y, bbMax.z), fMin, fMax);
	Project(axis, vec3(bbMin.x, bbMax.y, bbMax.z), fMin, fMax);
	Project(axis, vec3(bbMax.x, bbMax.y, bbMax.z), fMin, fMax);
}

void CCollisionMaster::Project(vec3& axis, vec3 vertex, _float& fMin, _float& fMax)
{
	_float val = dot(axis, vertex);
	if (val < fMin)
		fMin = val;
	if (val > fMax)
		fMax = val;
}

//int TestTriangleAABB(Point v0, Point v1, Point v2, AABB b)
//{
//	float p0, p1, p2, r;
//
//	// compute box center and extents (if not already given in that format)
//	// AABB박스의 중점과 크기 계산
//	Vector c = (b.min + b.max) * 0.5f;			// AABB의 center
//	float e0 = (b.max.x - b.min.x) * 0.5f;		// x축의 half length
//	float e1 = (b.max.y - b.min.y) * 0.5f;		// y축의 half length
//	float e2 = (b.max.z - b.min.z) * 0.5f;		// z축의 half length
//
//	// Translate triangle as conceptually moving AABB to origin
//	// 삼각형의 위치를 AABB의 원점으로 이동
//	v0 = v0 - c;
//	v1 = v1 - c;
//	v2 = v2 - c;
//
//	// Compute edge vectors for triangle
//	// 원점으로 옮겨진 삼각형의 세 변 계산
//	Vector f0 = v1 - v0;
//	Vector f1 = v2 - v1;
//	Vector f2 = v0 - v2;
//	
//	// Test axes a00..a22 (category 3)
//	// Test axis a00
//	p0 = v0.z * v1.y - v0.y * v1.z;
//	p2 = v2.z * (v1.y - v0.y) - v2.y * (v1.z - v0.z);
//	r = e1 * Abs(f0.z) + e2 * Abs(f0.y);
//	if (Max(-Max(p0, p2), Min(p0, p2)) > r)
//		return 0; // axis is a separating axis
//
//	// Repeat similar tests for remaining axes a01..a22
//	....;
//
//	// Test the three axes corresponding to the face normals of AABB b (category 1).
//	// Exit if ...
//	// ... [-e0, e0] and [min(v0.x, v1.x, v2.x), max(v0.x, v1.x, v2.x)] do not overlap
//	if (Max(v0.x, v1.x, v2.x) < -e0 || Min(v0.x, v1.x, v2.x) > e0) return 0;
//	// ... [-e1, e1] and [min(v0.y, v1.y, v2.y), max(v0.y, v1.y, v2.y)] do not overlap
//	if (Max(v0.y, v1.y, v2.y) < -e1 || Min(v0.y, v1.y, v2.y) > e1) return 0;
//	// ... [-e2, e2] and [min(v0.z, v1.z, v2.z), max(v0.z, v1.z, v2.z)] do not overlap
//	if (Max(v0.z, v1.z, v2.z) < -e2 || Min(v0.z, v1.z, v2.z) > e2) return 0;
//
//
//	// Test separating axis corresponding to triangle face normal (category 2)
//	Plane p;
//	p.n = Cross(f0, f1);
//	p.d = Dot(p.n, v0);
//	return TestAABBPlane(b, p);
//}
//
//int TestAABBPlane(AABB b, Plane p)
//{
//	// These two lines not necessary with a (Center, extents) AABB representation
//	Point c = (b.max + b.min) * 0.5f; // Compute AABB center
//	Point e = b.max - c; // Compute positive extents
//	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
//	float r = e[0] * Abs(p.n[0]) + e[1] * Abs(p.n[1]) + e[2](Abs(p.n[2]);
//	// Compute distance of box center from plane
//	float s = Dot(p.n, c) - p.d;
//	// Intersection occurs when distance s falls within [-r, +r] interval
//	return Abs(s) <= r;
//}