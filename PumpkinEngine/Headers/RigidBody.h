#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "iRigidBody.h"

NAMESPACE_BEGIN(Engine)

class CRigidBodyDesc;
class iShape;
class ENGINE_API CRigidBody : public iRigidBody
{
private: //From.Desc
	_bool			m_bIsStatic;
	_bool			m_bIsGround;

	_float			m_fMass;
	_float			m_fRestitution;
	_float			m_fFriction;
	_float			m_fLinearDamping;
	_float			m_fAngularDamping;

	glm::vec3		m_vPosition;
	glm::vec3		m_vLinearVelocity;
	glm::vec3		m_vLinearFactor;
	glm::vec3		m_vAngularFactor;
	glm::vec3		m_vAngularVelocity;
	
	glm::quat		m_qRotation;

private:
	_float			m_fInvMass;
	iShape*			m_pShape;
	glm::vec3		m_vPreviousPosition;
	glm::vec3		m_vForce;
	glm::vec3		m_vTorque;
	glm::vec3		m_vGravity;
	glm::vec3		m_vLinearAcceleration;
	glm::vec3		m_vAngularAcceleration;

	CRigidBodyDesc*	m_pDesc;


private:
	explicit CRigidBody();
	virtual ~CRigidBody();
	virtual void Destroy();

public:
	void Update(const _float& dt);
	void SetGravityAcceleration(const glm::vec3& gravity);
	void UpdateAcceleration();
	void VerletStep1(const _float& dt);
	void VerletStep2(const _float& dt);
	void VerletStep3(const _float& dt);
	void KillForces();
	void ApplyDamping(_float dt);

public:
	virtual glm::vec3 GetPosition();
	virtual void SetPosition(const glm::vec3& position);

	virtual glm::quat GetRotation();
	virtual void SetRotation(const glm::quat& rotation);

	virtual void ApplyForce(const glm::vec3& force);
	virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint);

	virtual void ApplyImpulse(const glm::vec3& impulse);
	virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint);

	virtual void ApplyTorque(const glm::vec3& torque);
	virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

public:
	glm::vec3 GetPreviousPosition()			{ return m_vPreviousPosition; }
	glm::vec3 GetLinearVelocity()			{ return m_vLinearVelocity; }
	glm::vec3 GetAngularVelocity()			{ return m_vAngularVelocity; }
	_float GetMass()						{ return m_fMass; }
	_float GetInvMass()						{ return m_fInvMass; }
	_float GetRestitution()					{ return m_fRestitution; }
	_float GetFriction()					{ return m_fFriction; }
	void SetLinearVelocity(glm::vec3 value) { m_vLinearVelocity = value; }

public:
	iShape* GetShape()			{ return m_pShape; }
	_bool IsStatic()			{ return m_bIsStatic; }
	_bool IsGround()			{ return m_bIsGround; }
	void ResetAll();

private:
	RESULT Ready(const CRigidBodyDesc& desc, iShape* shape);
	void SetRigidBodyDesc(const CRigidBodyDesc& desc);
public:
	static CRigidBody* Create(const CRigidBodyDesc& desc, iShape* shape);
};

NAMESPACE_END

#endif //_RIGIDBODY_H_