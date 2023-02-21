#ifndef _SPHERESHAPE_H_
#define _SPHERESHAPE_H_

#include "iShape.h"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CSphereShape : public iShape
{
private:
	_float			m_fRadius;

private:
	explicit CSphereShape();
	virtual ~CSphereShape();
	virtual void Destroy();

public:
	_float GetRadius()			{ return m_fRadius; }

private:
	RESULT Ready(eShapeType type, _float radius);
public:
	static CSphereShape* Create(eShapeType type, _float radius);
};

NAMESPACE_END

#endif //_SPHERESHAPE_H_
