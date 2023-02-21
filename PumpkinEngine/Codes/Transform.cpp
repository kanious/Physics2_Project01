#include "pch.h"
#include "../Headers/Transform.h"
#include "../Headers/Component.h"


USING(Engine)
USING(glm)

CTransform::CTransform()
	: m_matWorld(mat4x4(1.f))
	, m_vPos(vec3(0.f))
	, m_vRot(vec3(0.f))
	, m_vScale(vec3(1.f))
	, m_vRevolve((0.f))
	, m_pParentTransform(nullptr)
	, m_qRot(quat(vec3(0.f)))
{
}

CTransform::CTransform(const CTransform& rhs)
	: m_matWorld(mat4x4(1.f))
	, m_vPos(vec3(0.f))
	, m_vRot(vec3(0.f))
	, m_vScale(vec3(1.f))
	, m_vRevolve((0.f))
	, m_pParentTransform(nullptr)
	, m_qRot(quat(vec3(0.f)))
{
	m_tag = rhs.m_tag;
}

CTransform::~CTransform()
{
}

// Basic Update Function, calculate world matrix
void CTransform::Update(const _float& dt)
{
 	m_matWorld = mat4x4(1.f);

	/*m_matWorld = translate(m_matWorld, m_vPos);
	m_matWorld = rotate(m_matWorld, radians(m_vRot.z), vec3(0.f, 0.f, 1.f));
	m_matWorld = rotate(m_matWorld, radians(m_vRot.y), vec3(0.f, 1.f, 0.f));
	m_matWorld = rotate(m_matWorld, radians(m_vRot.x), vec3(1.f, 0.f, 0.f));
	m_matWorld = scale(m_matWorld, m_vScale);*/

	mat4x4 matTrans = mat4x4(1.f);
	mat4x4 matRot = mat4x4(1.f);
	mat4x4 matScale = mat4x4(1.f);
	matTrans = translate(matTrans, m_vPos);
	matRot = mat4_cast(m_qRot);
	matScale = scale(matScale, m_vScale);
	m_matWorld = matTrans * matRot * matScale;

	if (nullptr != m_pParentTransform)
	{
		mat4x4 matRevolve = mat4x4(1.f);
		matRevolve = rotate(matRevolve, radians(m_vRevolve.x), vec3(1.f, 0.f, 0.f));
		matRevolve = rotate(matRevolve, radians(m_vRevolve.y), vec3(0.f, 1.f, 0.f));
		matRevolve = rotate(matRevolve, radians(m_vRevolve.z), vec3(0.f, 0.f, 1.f));
		m_matWorld = matRevolve * m_matWorld;

		const mat4x4* matParent = m_pParentTransform->GetWorldMatrix();
		m_matWorld = *matParent * m_matWorld;
	}
}

// Call instead of destructor to manage class internal data
void CTransform::Destroy()
{
	CComponent::Destroy();
}

// Final position information calculated with all parent information
const vec3 CTransform::GetPositionWithParent()
{
	vec3 vPos = m_vPos;
	if (nullptr != m_pParentTransform)
		vPos += m_pParentTransform->GetPositionWithParent();

	return vPos;
}

// Final rotation information calculated with all parent information
const vec3 CTransform::GetRotationWithParent()
{
	vec3 vRot = m_vRot;
	if (nullptr != m_pParentTransform)
		vRot += m_pParentTransform->GetRotationWithParent();

	return vRot;
}

// Get parent world matrix information
const mat4x4 CTransform::GetParentMatrix()
{
	if (nullptr != m_pParentTransform)
		return *m_pParentTransform->GetWorldMatrix();

	return mat4x4(1.f);
}

// Set world matrix directly
void CTransform::SetWorldMatrix(mat4x4 matWorld)
{
	m_matWorld = matWorld;
}

// Set position/rotation/scale information
void CTransform::SetPosRotScale(vec3 vPos, vec3 vRot, vec3 vScale)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_vScale = vScale;
}

// Set position from vector
void CTransform::SetPosition(vec3 vPos)
{
	m_vPos = vPos;
}

// Set position X
void CTransform::SetPositionX(_float x)
{
	m_vPos.x = x;
}

// Set position Y
void CTransform::SetPositionY(_float y)
{
	m_vPos.y = y;
}

// Set position Z
void CTransform::SetPositionZ(_float z)
{
	m_vPos.z = z;
}

// Set rotation
void CTransform::SetRotation(vec3 vRot)
{
	m_vRot = vRot;
}

// Set rotation X
void CTransform::SetRotationX(_float x)
{
	m_vRot.x = x;
}

// Set rotation Y
void CTransform::SetRotationY(_float y)
{
	m_vRot.y = y;
}

// Set rotation Z
void CTransform::SetRotationZ(_float z)
{
	m_vRot.z = z;
}

// Set scale
void CTransform::SetScale(vec3 vScale)
{
	m_vScale = vScale;
}

// Set scale X
void CTransform::SetScaleX(_float x)
{
	m_vScale.x = x;
}

// Set scale Y
void CTransform::SetScaleY(_float y)
{
	m_vScale.y = y;
}

// Set scale Z
void CTransform::SetScaleZ(_float z)
{
	m_vScale.z = z;
}

// Set revolve information
void CTransform::SetRevolve(vec3 vRevolve)
{
	m_vRevolve = vRevolve;
}

// Set parent transform
void CTransform::SetParent(CTransform* pParent)
{
	m_pParentTransform = pParent;
}

void CTransform::SetQuaternion(glm::quat quaternion)
{
	m_qRot = quaternion;
}

// Add position
void CTransform::AddPosition(vec3 vPos)
{
	m_vPos += vPos;
}

// Add position X
void CTransform::AddPositionX(_float x)
{
	m_vPos.x += x;
}

// Add position Y
void CTransform::AddPositionY(_float y)
{
	m_vPos.y += y;
}

// Add position Z
void CTransform::AddPositionZ(_float z)
{
	m_vPos.z += z;
}

// Add rotation
void CTransform::AddRotation(vec3 vRot)
{
	m_vRot += vRot;
}

// Add rotation X
void CTransform::AddRotationX(_float x)
{
	m_vRot.x += x;
}

// Add rotation Y
void CTransform::AddRotationY(_float y)
{
	m_vRot.y += y;
}

// Add rotation Z
void CTransform::AddRotationZ(_float z)
{
	m_vRot.z += z;
}

// Add scale
void CTransform::AddScale(vec3 vScale)
{
	m_vScale += vScale;
}

// Add revolve
void CTransform::AddRevolve(vec3 vRevolve)
{
	m_vRevolve += vRevolve;
}

// calculate world matrix with my data
void CTransform::ComputeWorldMatrix(glm::mat4x4& matWorld, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale)
{
	//matWorld = translate(matWorld, m_vPos + vPos);
	//matWorld = rotate(matWorld, radians(m_vRot.z + vRot.z), vec3(0.f, 0.f, 1.f));
	//matWorld = rotate(matWorld, radians(m_vRot.y + vRot.y), vec3(0.f, 1.f, 0.f));
	//matWorld = rotate(matWorld, radians(m_vRot.x + vRot.x), vec3(1.f, 0.f, 0.f));
	//matWorld = scale(matWorld, m_vScale + vScale);

	mat4x4 matTrans = mat4x4(1.f);
	mat4x4 matRot = mat4x4(1.f);
	mat4x4 matScale = mat4x4(1.f);
	matTrans = translate(matTrans, m_vPos);
	matRot = mat4_cast(m_qRot);
	matScale = scale(matScale, m_vScale);
	matWorld = matTrans * matRot * matScale;
}

// Initialize transform
RESULT CTransform::Ready()
{
	m_tag = "Transform";

	return PK_NOERROR;
}

// Clone component
CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

// Create an instance
CTransform* CTransform::Create()
{
	CTransform* pInstance = new CTransform;
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();;
		pInstance = nullptr;
	}

	return pInstance;
}
