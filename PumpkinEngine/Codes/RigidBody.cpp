#include "pch.h"
#include "../Headers/RigidBody.h"
#include "../Headers/RigidBodyDesc.h"
#include "../Headers/iShape.h"
#include "../Headers/Transform.h"

USING(Engine)
USING(std)
USING(glm)

CRigidBody::CRigidBody()
	: m_pDesc(nullptr)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Destroy()
{
	if (nullptr != m_pDesc)
		delete m_pDesc;
}

void CRigidBody::Update(const _float& dt)
{
}

void CRigidBody::SetGravityAcceleration(const vec3& gravity)
{
	m_vGravity = gravity;
}

void CRigidBody::UpdateAcceleration()
{
	if (m_bIsStatic)
		return;

	m_vLinearAcceleration = m_vForce * m_fInvMass + m_vGravity;
	m_vAngularAcceleration = m_vTorque * m_fInvMass * m_fInvMass;
}

void CRigidBody::VerletStep1(const _float& dt)
{
	if (m_bIsStatic)
		return;

	m_vPreviousPosition = m_vPosition;
	m_vPosition += (m_vLinearVelocity + m_vLinearAcceleration * (dt/* * 0.5f*/)) * dt;

	if (-10.f > m_vPosition.y)
		m_vPosition.y = 5.f;

	vec3 axis = m_vAngularVelocity + m_vAngularAcceleration * dt;
	_float angle = length(axis);
	axis = normalize(axis);
	if (angle != 0.f)
	{
		quat rot = angleAxis(angle, axis);
		m_qRotation *= rot;
	}
}

void CRigidBody::VerletStep2(const _float& dt)
{
	if (m_bIsStatic) 
		return;

	m_vLinearVelocity += m_vLinearAcceleration * (dt * 0.5f);
	m_vAngularVelocity += m_vAngularAcceleration * (dt * 0.5f);
}

void CRigidBody::VerletStep3(const _float& dt)
{
	VerletStep2(dt);
}

void CRigidBody::KillForces()
{
	m_vForce = vec3(0.f);
	m_vTorque = vec3(0.f);
}

void CRigidBody::ApplyDamping(_float dt)
{
	m_vLinearVelocity *= pow(1.f - m_fLinearDamping, dt);
	m_vAngularVelocity *= m_fAngularDamping;// pow(1.f - m_fAngularDamping, dt);

	if (0.001f > length(m_vLinearVelocity))
		m_vLinearVelocity = vec3(0.f);
	if (0.001f > length(m_vAngularVelocity))
		m_vAngularVelocity = vec3(0.f);
}

vec3 CRigidBody::GetPosition()
{
	return m_vPosition;
}

void CRigidBody::SetPosition(const vec3& position)
{
	m_vPosition = position;
}

quat CRigidBody::GetRotation()
{
	return m_qRotation;
}

void CRigidBody::SetRotation(const quat& rotation)
{
	m_qRotation = rotation;
}

void CRigidBody::ApplyForce(const vec3& force)
{
	m_vForce += force;
}

void CRigidBody::ApplyForceAtPoint(const vec3& force, const vec3& relativePoint)
{
	ApplyForce(force);
	ApplyTorque(cross(relativePoint, force));
}

void CRigidBody::ApplyImpulse(const vec3& impulse)
{
	m_vLinearVelocity += impulse * m_fInvMass * m_fInvMass;
}

void CRigidBody::ApplyImpulseAtPoint(const vec3& impulse, const vec3& relativePoint)
{
	ApplyTorqueImpulse(cross(relativePoint, impulse));
}

void CRigidBody::ApplyTorque(const vec3& torque)
{
	m_vTorque += torque;
}

void CRigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
{
	m_vAngularVelocity += torqueImpulse;
}

void CRigidBody::ResetAll()
{
	SetRigidBodyDesc(*m_pDesc);
}

RESULT CRigidBody::Ready(const CRigidBodyDesc& desc, iShape* shape)
{
	m_pDesc = new CRigidBodyDesc(desc);

	SetRigidBodyDesc(desc);

	m_pShape = shape;

	return PK_NOERROR;
}

void CRigidBody::SetRigidBodyDesc(const CRigidBodyDesc& desc)
{
	m_bIsStatic = desc.isStatic;
	m_bIsGround = desc.isGround;

	if (m_bIsStatic || desc.mass <= 0.f)
	{
		m_fMass = 0.f;
		m_fInvMass = 0.f;
		m_bIsStatic = true;
	}
	else
	{
		m_fMass = desc.mass;
		m_fInvMass = 1.f / m_fMass;
	}

	m_fRestitution = desc.restitution;
	m_fFriction = desc.friction;
	m_fLinearDamping = desc.linearDamping;
	m_fAngularDamping = desc.angularDamping;

	m_vPosition = desc.position;
	m_vLinearVelocity = desc.linearVelocity;
	m_vLinearFactor = desc.linearFactor;
	m_vAngularVelocity = desc.angularVelocity;
	m_vAngularFactor = desc.angularFactor;

	m_qRotation = desc.rotation;
}

CRigidBody* CRigidBody::Create(const CRigidBodyDesc& desc, iShape* shape)
{
	CRigidBody* pInstance = new CRigidBody();
	if (PK_NOERROR != pInstance->Ready(desc, shape))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
