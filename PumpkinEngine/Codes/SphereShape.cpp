#include "pch.h"
#include "../Headers/SphereShape.h"

USING(Engine)
USING(std)

CSphereShape::CSphereShape()
    : m_fRadius(0.f)
{
}

CSphereShape::~CSphereShape()
{
}

void CSphereShape::Destroy()
{
}

RESULT CSphereShape::Ready(eShapeType type, _float radius)
{
    m_shapeType = type;
    m_fRadius = radius;
    return PK_NOERROR;
}

CSphereShape* CSphereShape::Create(eShapeType type, _float radius)
{
    CSphereShape* pInstance = new CSphereShape();
    if (PK_NOERROR != pInstance->Ready(type, radius))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
