#ifndef _IPHYSICSFACTORY_H_
#define _IPHYSICSFACTORY_H_

#include <functional>
#include "Base.h"

NAMESPACE_BEGIN(Engine)

class iPhysicsWorld;
class iRigidBody;
class CRigidBodyDesc;
class iShape;
class ENGINE_API iPhysicsFactory : public CBase
{
protected:
	explicit iPhysicsFactory() {}
	virtual ~iPhysicsFactory() {}
	virtual void Destroy() = 0;

public:
	virtual iPhysicsWorld* CreateWorld(std::function<void(void)> callback) = 0;
	virtual iRigidBody* CreateRigidBody(const CRigidBodyDesc& desc, iShape* shape) = 0;
};

NAMESPACE_END

#endif //_IPHYSICSFACTORY_H_