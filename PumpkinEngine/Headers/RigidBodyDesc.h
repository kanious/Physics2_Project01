#ifndef _RIGIDBODYDESC_H_
#define _RIGIDBODYDESC_H_

#include "EngineDefines.h"
#include "glm\vec3.hpp"
#include "glm\gtx\quaternion.hpp"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CRigidBodyDesc
{
public:
	_bool isStatic;
	_bool isGround;

	_float mass;
	_float restitution;
	_float friction;
	_float linearDamping;
	_float angularDamping;

	glm::vec3 position;
	glm::vec3 linearVelocity;
	glm::vec3 linearFactor;
	glm::vec3 angularFactor;
	glm::vec3 angularVelocity;

	glm::quat rotation;

public:
	explicit CRigidBodyDesc()
		: isStatic(false)
		, isGround(false)
		, mass(1.f)
		, restitution(0.6f)
		, friction(0.4f)
		, position(glm::vec3(0.f))
		, linearVelocity(glm::vec3(0.f))
		, linearFactor(glm::vec3(1.f))
		, angularFactor(glm::vec3(1.f))
		, angularVelocity(glm::vec3(0.f))
		, rotation(1.f, 0.f, 0.f, 0.f)
		, linearDamping(0.01f)
		, angularDamping(0.05f)
	{}

	explicit CRigidBodyDesc(const CRigidBodyDesc& rhs)
		: isStatic(rhs.isStatic)
		, isGround(rhs.isGround)
		, mass(rhs.mass)
		, restitution(rhs.restitution)
		, friction(rhs.friction)
		, position(rhs.position)
		, linearVelocity(rhs.linearVelocity)
		, linearFactor(rhs.linearFactor)
		, angularFactor(rhs.angularFactor)
		, angularVelocity(rhs.angularVelocity)
		, rotation(rhs.rotation)
		, linearDamping(rhs.linearDamping)
		, angularDamping(rhs.angularDamping)
	{}
};

NAMESPACE_END

#endif //_RIGIDBODYDESC_H_