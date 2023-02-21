#ifndef _PLANESHAPE_H_
#define _PLANESHAPE_H_

#include "iShape.h"
#include "glm\vec3.hpp"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CPlaneShape : public iShape
{
private:
	glm::vec3		m_vNormal;
	_float			m_fDotProduct;

private:
	explicit CPlaneShape();
	virtual ~CPlaneShape();
	virtual void Destroy();

public:
	glm::vec3 GetNormal()			{ return m_vNormal; }
	_float GetDotProduct()			{ return m_fDotProduct; }

private:
	RESULT Ready(eShapeType type, glm::vec3 vNormal, _float dot);
public:
	static CPlaneShape* Create(eShapeType type, glm::vec3 vNormal, _float dot);
};

NAMESPACE_END

#endif //_PLANESHAPE_H_
