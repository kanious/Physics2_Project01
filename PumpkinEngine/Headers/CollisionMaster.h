#ifndef _COLLISIONMASTER_H_
#define _COLLISIONMASTER_H_

#include "Base.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

class CBoundingBox;
class CTransform;
class CQuadTree;
class COctree;

// Class that manages all collision checks
class ENGINE_API CCollisionMaster : public CBase
{
	SINGLETON(CCollisionMaster)

private:
	explicit CCollisionMaster();
	virtual ~CCollisionMaster();
	void Destroy();

public:
	_bool IntersectRayToVirtualPlane(_float planeSize, glm::vec3& vOrigin, glm::vec3& vDir, glm::vec3& vDest);
	_bool IntersectRayToSphere(glm::vec3& vOrigin, glm::vec3& vDir, glm::vec3 sCenter, _float sphereRadius);
	_bool IntersectRayToBoundingBox(CBoundingBox* pBoundingBox, CTransform* pParentTransform, glm::vec3& vOrigin, glm::vec3& vDir);
private:
	_bool IntersectPointToTriangle(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& vOrigin, glm::vec3& vDir, glm::vec3& vDest);
	_bool PointInTriangle(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C);
	_bool PointInDotThree(glm::vec3 p1, glm::vec3 p2, glm::vec3 pA, glm::vec3 pB);

public:
	_bool IntersectRayToTriangles(CQuadTree* pQuadTree, glm::vec3& vMain, glm::vec3& vTarget, glm::vec3& vDest);
	_bool IsRayBlockedByTriangles(CQuadTree* pQuadTree, glm::vec3& vMain, glm::vec3& vTarget, glm::vec3& vDest);
	_bool IntersectCheckForProjectiles(CQuadTree* pQuadTree, glm::vec3& vMain, glm::vec3& vTarget, glm::vec3& vDest);
private:
	void GetCenter(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& center, _float& radius);
	_bool IntersectRaySphere(glm::vec3& vOrigin, glm::vec3& vDir, glm::vec3& center, _float& radius);

public:
	_bool IntersectTriangleToAABB(TRIANGLE* triangle, CBoundingBox* boundingBox);
	_bool IntersectTriangleToOBB(TRIANGLE* triangle, CBoundingBox* boundingBox);
	_bool IntersectOBBToAABB(CBoundingBox* obb, CBoundingBox* aabb);
private:
	void ProjectTriangle(TRIANGLE* triangle, glm::vec3& axis, _float& fMin, _float& fMax);
	void ProjectBox(glm::vec3& bbMin, glm::vec3& bbMax, glm::vec3& axis, _float& fMin, _float& fMax);
	void Project(glm::vec3& axis, glm::vec3 vertex, _float& fMin, _float& fMax);
};

NAMESPACE_END

#endif //_COLLISIONMASTER_H_