#include "pch.h"
#include "../Headers/CollisionHandler.h"
#include "../Headers/RigidBody.h"
#include "../Headers/SphereShape.h"
#include "../Headers/PlaneShape.h"
#include "glm\gtx\projection.hpp"

USING(Engine)
USING(std)
USING(glm)

CCollisionHandler::CCollisionHandler()
{
}

CCollisionHandler::~CCollisionHandler()
{
}

void CCollisionHandler::Destroy()
{
}

void CCollisionHandler::Collide(const _float& dt, std::vector<CRigidBody*>& bodies, std::vector<sColPair>& vecCols)
{
	_bool IsCollided = false;

	for (int idxA = 0; idxA < bodies.size(); ++idxA)
	{
		CRigidBody* bodyA = bodies[idxA];
		iShape* shapeA = bodyA->GetShape();
		for (int idxB = idxA + 1; idxB < bodies.size(); ++idxB)
		{
			CRigidBody* bodyB = bodies[idxB];
			iShape* shapeB = bodyB->GetShape();

			switch (shapeA->GetShapeType())
			{
			case eShapeType::Sphere:
				if (eShapeType::Sphere == shapeB->GetShapeType())
				{
					IsCollided = CollideSphereSphere(dt,
						bodyA, dynamic_cast<CSphereShape*>(shapeA),
						bodyB, dynamic_cast<CSphereShape*>(shapeB));
				}
				else if (eShapeType::Plane == shapeB->GetShapeType())
				{
					if (bodyB->IsGround())
					{
						IsCollided = CollideSphereGroundPlane(dt,
							bodyA, dynamic_cast<CSphereShape*>(shapeA),
							bodyB, dynamic_cast<CPlaneShape*>(shapeB));
					}
					else
					{
						IsCollided = CollideSphereWallPlane(dt,
							bodyA, dynamic_cast<CSphereShape*>(shapeA),
							bodyB, dynamic_cast<CPlaneShape*>(shapeB));
					}
				}
				break;

			case eShapeType::Plane:
				if (eShapeType::Sphere == shapeB->GetShapeType())
				{
					if (bodyA->IsGround())
					{
						IsCollided = CollideSphereGroundPlane(dt,
							bodyB, dynamic_cast<CSphereShape*>(shapeB),
							bodyA, dynamic_cast<CPlaneShape*>(shapeA));
					}
					else
					{
						IsCollided = CollideSphereWallPlane(dt,
							bodyB, dynamic_cast<CSphereShape*>(shapeB),
							bodyA, dynamic_cast<CPlaneShape*>(shapeA));
					}
				}
				break;
			}

			if (IsCollided)
				vecCols.push_back(sColPair(bodyA, bodyB));

		}// for idxB
	}// for idxA
}

_bool CCollisionHandler::CollideSphereSphere(const _float& dt, CRigidBody* bodyA, CSphereShape* sphereA, CRigidBody* bodyB, CSphereShape* sphereB)
{
	if (bodyA->IsStatic() && bodyB->IsStatic())
		return false;

	vec3 posA = bodyA->GetPosition();
	vec3 posB = bodyB->GetPosition();
	vec3 preposA = bodyA->GetPreviousPosition();
	vec3 preposB = bodyB->GetPreviousPosition();
	_float rA = sphereA->GetRadius();
	_float rB = sphereB->GetRadius();

	vec3 v0 = posA - preposA;
	vec3 v1 = posB - preposB;
	_float t = 0.f;

	if (!TestMovingSphereSphere(preposA, rA, preposB, rB, v0, v1, t))
		return false; // No collision or future collision

	if (t > dt)
		return false; // No collision for this time step

	// Collision Resolution
	_float fractDT = t / (length(v0) + length(v1));
	_float revDT = (1.f - fractDT) * dt;
	bodyA->VerletStep1(-revDT);
	bodyB->VerletStep1(-revDT);

	posA = bodyA->GetPosition();
	posB = bodyB->GetPosition();
	preposA = bodyA->GetPreviousPosition();
	preposB = bodyB->GetPreviousPosition();

	_float massA = bodyA->GetMass();
	_float massB = bodyB->GetMass();
	_float invMassA = bodyA->GetInvMass();
	_float invMassB = bodyB->GetInvMass();
	_float restitutionA = bodyA->GetRestitution();
	_float restitutionB = bodyB->GetRestitution();
	vec3 linearVelocityA = bodyA->GetLinearVelocity();
	vec3 linearVelocityB = bodyB->GetLinearVelocity();

	_float totalMass = massA + massB;
	_float aFactor = massB / totalMass;
	_float bFactor = massA / totalMass;

	vec3 aMomentum = linearVelocityA * massA;
	vec3 bMomentum = linearVelocityB * massB;
	vec3 totalMomentum = aMomentum + bMomentum;

	aMomentum = totalMomentum * aFactor;
	bMomentum = totalMomentum * bFactor;

	vec3 dir = posB - posA;
	_float len = length(dir);
	_float overlap = len - (rA + rB);

	if (overlap < numeric_limits<_float>::epsilon())
	{
		vec3 overlapDir = dir;
		overlapDir = normalize(overlapDir);
		overlapDir *= -overlap;

		if (!bodyA->IsStatic())
		{
			posA -= overlapDir * aFactor;
			bodyA->SetPosition(posA);
		}
		if (!bodyB->IsStatic())
		{
			posB += overlapDir * bFactor;
			bodyB->SetPosition(posB);
		}

		dir = posB - posA;
		len = length(dir);
	}
	dir /= len;

	_float elasticity = 0.4f;

	vec3 aElasticMomentum = dir * (length(aMomentum) * elasticity);
	vec3 aInelasticMomentum = dir * length(aMomentum) * (1.f - elasticity);

	vec3 bElasticMomentum = dir * (length(bMomentum) * elasticity) * -1.f;
	vec3 bInelasticMomentum = dir * length(bMomentum) * (1.f - elasticity);

	linearVelocityA -= (aElasticMomentum + aInelasticMomentum) * invMassA * restitutionA;
	bodyA->SetLinearVelocity(linearVelocityA);
	linearVelocityB += (bElasticMomentum + bInelasticMomentum) * invMassB * restitutionB;
	bodyB->SetLinearVelocity(linearVelocityB);

	bodyA->VerletStep1(revDT);
	bodyB->VerletStep1(revDT);

	return true;
}

_bool CCollisionHandler::CollideSphereGroundPlane(const _float& dt, CRigidBody* sphereBody, CSphereShape* sphereShape, CRigidBody* planeBody, CPlaneShape* planeShape)
{
	if (sphereBody->IsStatic() && planeBody->IsStatic())
		return false;

	vec3 vSpherePos = sphereBody->GetPosition();
	vec3 vNormal = planeShape->GetNormal();
	_float fRadius = sphereShape->GetRadius();
	_float fDP = planeShape->GetDotProduct();

	if (!TestMovingSpherePlane(sphereBody->GetPreviousPosition(), vSpherePos, vNormal, fRadius, fDP))
	{
		return false;
	}

	vec3 vLinearVelocity = sphereBody->GetLinearVelocity();
	vec3 vAngularVelocity = sphereBody->GetAngularVelocity();

	// Collision Resolution
	vec3 closestPt = ClosestPtPointPlane(vSpherePos, vNormal, fDP);
	vec3 overlapVec = closestPt - vSpherePos;
	_float overlapLen = length(overlapVec);
	_float linearVelocityLen = length(vLinearVelocity);
	_float angularVelocityLen = length(vAngularVelocity);

	if (0.f < linearVelocityLen || 0.f < angularVelocityLen)
	{
		_float velocity = linearVelocityLen;
		_float fractDt = 0.f;
		if (0.f != velocity)
			fractDt = fRadius * ((fRadius / overlapLen) - 1.f) / velocity;
		_float partialDt = (1.f - fractDt) * dt;

		sphereBody->VerletStep1(-partialDt);
		vSpherePos = sphereBody->GetPosition();

		vec3 prevVelocity = vLinearVelocity;
		vec3 ref = reflect(vLinearVelocity, vNormal);
		vLinearVelocity = ref;
		sphereBody->SetLinearVelocity(vLinearVelocity);

		vec3 impactComponent = proj(vLinearVelocity, vNormal);
		vec3 impactTangent = vLinearVelocity - impactComponent;
		
		vec3 relativePt = normalize(closestPt - vSpherePos) * fRadius;
		_float surfaceVelocity = fRadius * angularVelocityLen;
		vec3 rotDir = normalize(cross(relativePt - vSpherePos, vAngularVelocity));

		_float sphereMass = sphereBody->GetMass();
		_float planeFriction = planeBody->GetFriction();

		if (0.f < dot(impactTangent, vNormal))
			sphereBody->ApplyImpulseAtPoint(-2.f * impactComponent * sphereMass, relativePt);
		else
		{
			vec3 impactForce = impactTangent * -1.f * sphereMass * planeFriction;
			sphereBody->ApplyForceAtPoint(impactForce, relativePt);
		}

		if (0.f == dot(vLinearVelocity, vNormal))
		{
			vec3 force = surfaceVelocity * rotDir * sphereMass * planeFriction;
			sphereBody->ApplyForce(force);
		}

		sphereBody->UpdateAcceleration();
		sphereBody->VerletStep1(partialDt);

		vSpherePos = sphereBody->GetPosition();
		closestPt = ClosestPtPointPlane(vSpherePos, vNormal, fDP);
		overlapVec = closestPt - vSpherePos;
		overlapLen = length(overlapVec);
		vLinearVelocity = sphereBody->GetLinearVelocity();

		if (overlapLen < fRadius)
		{
			vSpherePos += vNormal * (fRadius - overlapLen);
			sphereBody->SetPosition(vSpherePos);
			_float velocityDotNormal = dot(vLinearVelocity, vNormal);
			if (0.f > velocityDotNormal)
			{
				vLinearVelocity -= vNormal * velocityDotNormal;
				sphereBody->SetLinearVelocity(vLinearVelocity);
			}
		}
		else
		{
			vLinearVelocity *= sphereBody->GetRestitution();
			sphereBody->SetLinearVelocity(vLinearVelocity);
		}

	}
	else
		return false;

	return true;
}

_bool CCollisionHandler::CollideSphereWallPlane(const _float& dt, CRigidBody* sphereBody, CSphereShape* sphereShape, CRigidBody* planeBody, CPlaneShape* planeShape)
{
	if (sphereBody->IsStatic() && planeBody->IsStatic())
		return false;

	vec3 vSpherePos = sphereBody->GetPosition();
	vec3 vNormal = planeShape->GetNormal();
	_float fRadius = sphereShape->GetRadius();
	vec3 vPlanePos = planeBody->GetPosition();

	if (vNormal.x > 0)
	{
		if (vSpherePos.x - fRadius < vPlanePos.x)
		{
			vec3 vLinearVelocity = sphereBody->GetLinearVelocity();
			vec3 ref = reflect(vLinearVelocity, vNormal);
			vLinearVelocity = ref;
			sphereBody->SetLinearVelocity(vLinearVelocity);
		}
	}
	else if (vNormal.x < 0)
	{
		if (vSpherePos.x + fRadius > vPlanePos.x)
		{
			vec3 vLinearVelocity = sphereBody->GetLinearVelocity();
			vec3 ref = reflect(vLinearVelocity, vNormal);
			vLinearVelocity = ref;
			sphereBody->SetLinearVelocity(vLinearVelocity);
		}
	}
	else if (vNormal.z > 0)
	{
		if (vSpherePos.z - fRadius < vPlanePos.z)
		{
			vec3 vLinearVelocity = sphereBody->GetLinearVelocity();
			vec3 ref = reflect(vLinearVelocity, vNormal);
			vLinearVelocity = ref;
			sphereBody->SetLinearVelocity(vLinearVelocity);
		}
	}
	else if (vNormal.z < 0)
	{
		if (vSpherePos.z + fRadius > vPlanePos.z)
		{
			vec3 vLinearVelocity = sphereBody->GetLinearVelocity();
			vec3 ref = reflect(vLinearVelocity, vNormal);
			vLinearVelocity = ref;
			sphereBody->SetLinearVelocity(vLinearVelocity);
		}
	}

	return false;

}

_bool CCollisionHandler::TestMovingSphereSphere(
	const vec3& s0Center, const _float s0Radius,
	const vec3& s1Center, const _float s1Radius, const vec3& v0, const vec3& v1, _float& t)
{
	vec3 s = s1Center - s0Center;
	vec3 v = v1 - v0;
	_float r = s0Radius + s1Radius;
	_float c = dot(s, s) - r * r;

	if (c < 0.f)
	{
		t = 0.f;
		return true;
	}

	_float a = dot(v, v);
	if (a < numeric_limits<_float>::epsilon())
		return false; // Spheres are not moving relative to each other

	_float b = dot(v, s);
	if (b >= 0.f)
		return false; // Spheres are not moving towards each other

	_float d = (b * b) - (a * c);
	if (d < 0.f)
		return false; // Spheres do not intersect

	t = (-b - sqrt(d)) / a;

	return true;
}

_bool CCollisionHandler::TestMovingSpherePlane(
	const vec3& prevPosition, const vec3& curPosition,const vec3& normal,
	_float radius, _float dotProduct)
{
	_float fDistA = dot(prevPosition, normal) - dotProduct;
	_float fDistB = dot(curPosition, normal) - dotProduct;

	if (fDistA * fDistB < 0.f)
		return true; // different sides of a plane

	if (abs(fDistA) < radius || abs(fDistB) <= radius)
		return true; // start or end position within radius from plane

	return false;
}

vec3 CCollisionHandler::ClosestPtPointPlane(const vec3& pt, const vec3& planeNormal, _float planeDP)
{
	_float t = dot(planeNormal, pt) - planeDP;
	return pt - (t * planeNormal);
}

RESULT CCollisionHandler::Ready()
{
	return PK_NOERROR;
}

CCollisionHandler* CCollisionHandler::Create()
{
	CCollisionHandler* pInstance = new CCollisionHandler();
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
