#ifndef _PHYSICSWORLD_H_
#define _PHYSICSWORLD_H_

#include <functional>
#include "iPhysicsWorld.h"
#include "CollisionHandler.h"

NAMESPACE_BEGIN(Engine)

class CRigidBody;
class CCollisionHandler;
class ENGINE_API CPhysicsWorld : public iPhysicsWorld
{
private:
	glm::vec3						m_vGravity;
	std::vector<CRigidBody*>		m_vecRigidBodies;
	CCollisionHandler*				m_pColHandler;

	std::function<void(void)>		m_collisionCallback;

private:
	explicit CPhysicsWorld();
	virtual ~CPhysicsWorld();
	virtual void Destroy();

public:
	virtual void Update(const _float& dt);

public:
	virtual void SetGravity(const glm::vec3& gravity);
	virtual void AddBody(iRigidBody* body);
	virtual void RemoveBody(iRigidBody* body);
	virtual void ResetAllRigidBodies();
	virtual void ApplyRandomForce();

private:
	RESULT Ready(std::function<void(void)> callback);
public:
	static CPhysicsWorld* Create(std::function<void(void)> callback);
};

NAMESPACE_END

#endif //_PHYSICSWORLD_H_