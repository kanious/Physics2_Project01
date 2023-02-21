#ifndef _ISHAPE_H_
#define _ISHAPE_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

enum class eShapeType
{
	Box,
	Cylinder,
	Ghost,
	Plane,
	Sphere,
};

class ENGINE_API iShape: public CBase
{
protected:
	eShapeType			m_shapeType;

protected:
	explicit iShape() {}
	virtual ~iShape() {}
	virtual void Destroy() = 0;

public:
	eShapeType GetShapeType()	{ return m_shapeType; }
};

NAMESPACE_END

#endif //_ISHAPE_H_