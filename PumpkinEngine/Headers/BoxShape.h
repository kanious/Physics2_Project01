#ifndef _BOXSHAPE_H_
#define _BOXSHAPE_H_

#include "iShape.h"
#include "glm\vec3.hpp"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CBoxShape : public iShape
{
private:
	glm::vec3		m_vHalfExtents;

private:
	explicit CBoxShape();
	virtual ~CBoxShape();
	virtual void Destroy();

public:
	glm::vec3 GetHalfExtents()	{ return m_vHalfExtents; }

private:
	RESULT Ready(eShapeType type, glm::vec3 vHalf);
public:
	static CBoxShape* Create(eShapeType type, glm::vec3 vHalf);
};

NAMESPACE_END

#endif //_BOXSHAPE_H_
