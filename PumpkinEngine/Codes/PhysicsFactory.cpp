#include "pch.h"
#include "../Headers/PhysicsFactory.h"
#include "../Headers/PhysicsWorld.h"
#include "../Headers/RigidBody.h"
#include "../Headers/RigidBodyDesc.h"

USING(Engine)
USING(std)

CPhysicsFactory::CPhysicsFactory()
	: iPhysicsFactory()
{
}

CPhysicsFactory::~CPhysicsFactory()
{
}

void CPhysicsFactory::Destroy()
{
}

iPhysicsWorld* CPhysicsFactory::CreateWorld(function<void(void)> callback)
{
	return CPhysicsWorld::Create(callback);
}

iRigidBody* CPhysicsFactory::CreateRigidBody(const CRigidBodyDesc& desc, iShape* shape)
{
	return CRigidBody::Create(desc, shape);
}

RESULT CPhysicsFactory::Ready()
{
	return PK_NOERROR;
}

CPhysicsFactory* CPhysicsFactory::Create()
{
	CPhysicsFactory* pInstance = new CPhysicsFactory();
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
