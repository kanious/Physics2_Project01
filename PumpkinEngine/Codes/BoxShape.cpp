#include "pch.h"
#include "../Headers/BoxShape.h"

USING(Engine)
USING(std)
USING(glm)

CBoxShape::CBoxShape()
    : m_vHalfExtents(vec3(0.f))
{
}

CBoxShape::~CBoxShape()
{
}

void CBoxShape::Destroy()
{
}

RESULT CBoxShape::Ready(eShapeType type, vec3 vHalf)
{
    m_shapeType = type;
    m_vHalfExtents = vHalf;

    return PK_NOERROR;
}

CBoxShape* CBoxShape::Create(eShapeType type, vec3 vHalf)
{
    CBoxShape* pInstance = new CBoxShape();
    if (PK_NOERROR != pInstance->Ready(type, vHalf))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
