#include "pch.h"
#include "../Headers/PlaneShape.h"

USING(Engine)
USING(std)
USING(glm)

CPlaneShape::CPlaneShape()
    : m_vNormal(vec3(0.f)), m_fDotProduct(0.f)
{
}

CPlaneShape::~CPlaneShape()
{
}

void CPlaneShape::Destroy()
{
}

RESULT CPlaneShape::Ready(eShapeType type, vec3 vNormal, _float dot)
{
    m_shapeType = type;
    m_vNormal = vNormal;
    m_fDotProduct = dot;

    return PK_NOERROR;
}

CPlaneShape* CPlaneShape::Create(eShapeType type, vec3 vNormal, _float dot)
{
    CPlaneShape* pInstance = new CPlaneShape();
    if (PK_NOERROR != pInstance->Ready(type, vNormal, dot))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
