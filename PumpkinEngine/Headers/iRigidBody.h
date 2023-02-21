#ifndef _IRIGIDBODY_H_
#define _IRIGIDBODY_H_

#include "Base.h"
#include "glm\vec3.hpp"
#include "glm\gtx\quaternion.hpp"

NAMESPACE_BEGIN(Engine)

class CTransform;
class ENGINE_API iRigidBody : public CBase
{
protected:
	explicit iRigidBody() {}
	virtual ~iRigidBody() {}
	virtual void Destroy() = 0;

protected:
	CTransform* m_pParentTransform;
public:
	void SetTransform(CTransform* transform) { m_pParentTransform = transform; }

public:
	virtual glm::vec3 GetPosition() = 0;
	virtual void SetPosition(const glm::vec3& position) = 0;

	virtual glm::quat GetRotation() = 0;
	virtual void SetRotation(const glm::quat& rotation) = 0;

	virtual void ApplyForce(const glm::vec3& force) = 0;
	virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint) = 0;

	virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
	virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) = 0;

	virtual void ApplyTorque(const glm::vec3& torque) = 0;
	virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse) = 0;

};

NAMESPACE_END

#endif //_IRIGIDBODY_H_