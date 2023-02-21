#include "pch.h"
#include "../Headers/PhysicsWorld.h"
#include "../Headers/iRigidBody.h"
#include "../Headers/RigidBody.h"
#include "../Headers/CollisionHandler.h"
#include "../Headers/iShape.h"
#include "../Headers/EngineFunction.h"

USING(Engine)
USING(std)
USING(glm)

CPhysicsWorld::CPhysicsWorld()
	: m_vGravity(vec3(0.f)), m_collisionCallback(nullptr)
{
	m_vecRigidBodies.clear();
}

CPhysicsWorld::~CPhysicsWorld()
{
}

void CPhysicsWorld::Destroy()
{
	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
		SafeDestroy(m_vecRigidBodies[i]);

	SafeDestroy(m_pColHandler);
}

void CPhysicsWorld::Update(const _float& dt)
{
	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
		m_vecRigidBodies[i]->Update(dt);

	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
	{
		m_vecRigidBodies[i]->SetGravityAcceleration(m_vGravity);
		m_vecRigidBodies[i]->UpdateAcceleration();
	}

	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
	{
		m_vecRigidBodies[i]->VerletStep3(dt);
		m_vecRigidBodies[i]->ApplyDamping(dt / 2.f);
	}

	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
		m_vecRigidBodies[i]->VerletStep1(dt);

	// Collision
	vector<CCollisionHandler::sColPair> vecPairs;
	m_pColHandler->Collide(dt, m_vecRigidBodies, vecPairs);

	for (int i = 0; i < vecPairs.size(); ++i)
	{
		CCollisionHandler::sColPair pair = vecPairs[i];
		if (eShapeType::Plane == pair.pBodyA->GetShape()->GetShapeType() ||
			eShapeType::Plane == pair.pBodyB->GetShape()->GetShapeType())
			continue;

		if (nullptr != m_collisionCallback)
			m_collisionCallback();
	}

	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
	{
		m_vecRigidBodies[i]->VerletStep2(dt);
		m_vecRigidBodies[i]->ApplyDamping(dt / 2.f);
		m_vecRigidBodies[i]->KillForces();
	}
}

void CPhysicsWorld::SetGravity(const vec3& gravity)
{
	m_vGravity = gravity;
}

void CPhysicsWorld::AddBody(iRigidBody* body)
{
	if (nullptr == body)
		return;

	CRigidBody* rigidBody = dynamic_cast<CRigidBody*>(body);

	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
	{
		if (rigidBody == m_vecRigidBodies[i])
			return;
	}

	m_vecRigidBodies.push_back(rigidBody);
}

void CPhysicsWorld::RemoveBody(iRigidBody* body)
{
	CRigidBody* rigidBody = dynamic_cast<CRigidBody*>(body);

	vector<CRigidBody*>::iterator iter;
	for (iter = m_vecRigidBodies.begin(); iter != m_vecRigidBodies.end(); ++iter)
	{
		if (rigidBody == (*iter))
		{
			SafeDestroy(*iter);
			m_vecRigidBodies.erase(iter);
			return;
		}
	}
}

void CPhysicsWorld::ResetAllRigidBodies()
{
	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
		m_vecRigidBodies[i]->ResetAll();
}

void CPhysicsWorld::ApplyRandomForce()
{
	for (int i = 0; i < m_vecRigidBodies.size(); ++i)
	{
		_float randX = GetRandNum(-50, 50);
		_float randZ = GetRandNum(-50, 50);
		_float mass = m_vecRigidBodies[i]->GetMass();
		m_vecRigidBodies[i]->ApplyImpulse(vec3(randX * mass, 0.f, randZ * mass));
	}
}

RESULT CPhysicsWorld::Ready(function<void(void)> callback)
{
	m_pColHandler = CCollisionHandler::Create();

	m_collisionCallback = callback;

	return PK_NOERROR;
}

CPhysicsWorld* CPhysicsWorld::Create(function<void(void)> callback)
{
	CPhysicsWorld* pInstance = new CPhysicsWorld();
	if (PK_NOERROR != pInstance->Ready(callback))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
