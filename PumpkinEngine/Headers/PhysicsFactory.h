#ifndef _PHYSICSFACTORY_H_
#define _PHYSICSFACTORY_H_

#include "iPhysicsFactory.h"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CPhysicsFactory : public iPhysicsFactory
{
private:
	explicit CPhysicsFactory();
	virtual ~CPhysicsFactory();
	virtual void Destroy();

public:
	virtual iPhysicsWorld* CreateWorld(std::function<void(void)> callback);
	virtual iRigidBody* CreateRigidBody(const CRigidBodyDesc& desc, iShape* shape);

private:
	RESULT Ready();
public:
	static CPhysicsFactory* Create();
};

NAMESPACE_END

#endif //_PHYSICSFACTORY_H_