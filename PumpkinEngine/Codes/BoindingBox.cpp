#include "pch.h"
#include "..\Headers\BoundingBox.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\VIBuffer.h"
#include "..\Headers\Shader.h"
#include "..\Headers\Transform.h"
#include "..\Headers\ComponentMaster.h"
#include "..\Headers\Component.h"
#include "..\Headers\OpenGLDevice.h"

USING(Engine)
USING(glm)
USING(std)

CBoundingBox::CBoundingBox()
	: m_pVIBuffer(nullptr), m_pVertices(nullptr)
	, m_vCenter(vec3(0.f)), m_vHalfExtents(vec3(0.f))
	, m_vMin(vec3(0.f)), m_vMax(vec3(0.f)), m_vMinWorld(vec3(0.f)), m_vMaxWorld(vec3(0.f))
	, m_pOpenGLDevice(COpenGLDevice::GetInstance())
	, m_pShader(nullptr)
	, m_pParentTransform(nullptr)
	, m_shaderID("")
{
	m_pOpenGLDevice->AddRefCnt();
}

CBoundingBox::CBoundingBox(const CBoundingBox& rhs)
	: m_pVIBuffer(rhs.m_pVIBuffer), m_pVertices(rhs.m_pVertices)
	, m_vCenter(rhs.m_vCenter), m_vHalfExtents(rhs.m_vHalfExtents)
	, m_vMin(rhs.m_vMin), m_vMax(rhs.m_vMax), m_vMinWorld(rhs.m_vMinWorld), m_vMaxWorld(rhs.m_vMaxWorld)
	, m_pOpenGLDevice(rhs.m_pOpenGLDevice)
	, m_pShader(rhs.m_pShader)
	, m_pParentTransform(nullptr)
	, m_shaderID(rhs.m_shaderID)
{
	m_tag = rhs.m_tag;
	m_pOpenGLDevice->AddRefCnt();
	if (nullptr != m_pVIBuffer) m_pVIBuffer->AddRefCnt();
	if (nullptr != m_pShader) m_pShader->AddRefCnt();
}

CBoundingBox::~CBoundingBox()
{
}

// Basic Render Function, render bounding box
void CBoundingBox::Render()
{
	if (!m_bEnable)
		return;

	if (nullptr == m_pParentTransform || nullptr == m_pShader)
		return;

	mat4x4 matWorld = *m_pParentTransform->GetWorldMatrix();
	const mat4x4 matView = m_pOpenGLDevice->GetViewMatrix();
	const mat4x4 matProj = m_pOpenGLDevice->GetProjMatrix();
	m_pShader->SetMatrixInfo(matWorld, matView, matProj);

 	if (nullptr != m_pVIBuffer)
		m_pVIBuffer->Render();
}

// Render bounding box without parent information
void CBoundingBox::RenderWithoutParent()
{
	mat4x4 matWorld(1.f);
	const mat4x4 matView = m_pOpenGLDevice->GetViewMatrix();
	const mat4x4 matProj = m_pOpenGLDevice->GetProjMatrix();
	m_pShader->SetMatrixInfo(matWorld, matView, matProj);
	
	if (nullptr != m_pVIBuffer)
		m_pVIBuffer->Render();
}

// Render bounding box with another parent information
void CBoundingBox::RenderWithParent(CTransform* parent)
{
	if (nullptr == parent)
		return;

	mat4x4 matWorld = *parent->GetWorldMatrix();
	const mat4x4 matView = m_pOpenGLDevice->GetViewMatrix();
	const mat4x4 matProj = m_pOpenGLDevice->GetProjMatrix();
	m_pShader->SetMatrixInfo(matWorld, matView, matProj);

	if (nullptr != m_pVIBuffer)
		m_pVIBuffer->Render();
}

// Call instead of destructor to manage class internal data
void CBoundingBox::Destroy()
{
	delete[] m_pVertices;
	SafeDestroy(m_pVIBuffer);
	SafeDestroy(m_pOpenGLDevice);
	SafeDestroy(m_pShader);
	m_pParentTransform = nullptr;

	CComponent::Destroy();
}

// Update bounding box information by parent information
void CBoundingBox::UpdateBoundingBox(mat4x4& parentWorldMatrix)
{
	m_vMax = vec3(-100000.f);
	m_vMin = vec3(100000.f);

	vec3 tempPos;
	mat4x4 matWorld = mat4x4(1.f);
	for (int i = 0; i < 8; ++i)
	{
		matWorld[3] = m_pVertices[i].vPos;
		matWorld = parentWorldMatrix * matWorld;
		tempPos = matWorld[3];

		if (tempPos.x > m_vMax.x)
			m_vMax.x = tempPos.x;

		if (tempPos.y > m_vMax.y)
			m_vMax.y = tempPos.y;

		if (tempPos.z > m_vMax.z)
			m_vMax.z = tempPos.z;

		if (tempPos.x < m_vMin.x)
			m_vMin.x = tempPos.x;

		if (tempPos.y < m_vMin.y)
			m_vMin.y = tempPos.y;

		if (tempPos.z < m_vMin.z)
			m_vMin.z = tempPos.z;
	}

	m_vHalfExtents = (m_vMax - m_vMin) / 2.f;
	m_vCenter = m_vMin + m_vHalfExtents;
}

// Set bounding box color
void CBoundingBox::SetColor(glm::vec3 vColor)
{
	if (nullptr != m_pShader)
		m_pShader->SetColor(vColor);
}

// Initialize bounding box
RESULT CBoundingBox::Ready(vec3 min, vec3 max, string shaderID)
{
	m_tag = "BBox";

	m_vMin = vec3(min.x - 0.01f, min.y - 0.01f, min.z - 0.01f);
	m_vMax = vec3(max.x + 0.01f, max.y + 0.01f, max.z + 0.01f);
	m_vHalfExtents = (m_vMax - m_vMin) / 2.f;
	m_vCenter = m_vMin + m_vHalfExtents;

	m_vOriginMin = m_vMin;
	m_vOriginMax = m_vMax;
	m_vOriginHalfExtents = m_vHalfExtents;
	m_vOriginCenter = m_vCenter;
	
	m_vMinWorld = m_vMin;
	m_vMaxWorld = m_vMax;
	
	// Buffer
    _uint vertexNum = 8;
    _uint indexNum = 36;

	m_pVertices = new VTX[vertexNum];
    memset(m_pVertices, 0, sizeof(*m_pVertices));
	m_pVertices[0].vPos = vec4(m_vMin.x, m_vMax.y, m_vMax.z, 1.f);
	m_pVertices[1].vPos = vec4(m_vMin.x, m_vMax.y, m_vMin.z, 1.f);
	m_pVertices[2].vPos = vec4(m_vMax.x, m_vMax.y, m_vMin.z, 1.f);
	m_pVertices[3].vPos = vec4(m_vMax.x, m_vMax.y, m_vMax.z, 1.f);
	m_pVertices[4].vPos = vec4(m_vMin.x, m_vMin.y, m_vMax.z, 1.f);
	m_pVertices[5].vPos = vec4(m_vMin.x, m_vMin.y, m_vMin.z, 1.f);
	m_pVertices[6].vPos = vec4(m_vMax.x, m_vMin.y, m_vMin.z, 1.f);
	m_pVertices[7].vPos = vec4(m_vMax.x, m_vMin.y, m_vMax.z, 1.f);

	_uint* pIndices = new _uint[indexNum]
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 5,
		0, 5 ,1,
		2, 6, 7,
		2, 7, 3,
		1, 5, 6,
		1, 6, 2,
		3, 7, 4,
		3, 4, 0,
		5, 4, 7,
		5, 7, 6
	};

	m_pVIBuffer = CVIBuffer::Create(vertexNum, m_pVertices, indexNum, pIndices, xyz_index);
	if (nullptr != m_pVIBuffer)
		m_pVIBuffer->SetWireFrame(true);

	delete[] pIndices;

	m_shaderID = shaderID;
	CComponent* pComponent = CloneComponent<CShader*>(shaderID);
	if (nullptr != pComponent)
		m_pShader = dynamic_cast<CShader*>(pComponent);

	return PK_NOERROR;
}

// Clone component
CComponent* CBoundingBox::Clone()
{
	return new CBoundingBox(*this);
}

// Create an instance
CBoundingBox* CBoundingBox::Create(vec3 min, vec3 max, string shaderID)
{
	CBoundingBox* pInstance = new CBoundingBox;
	if (PK_NOERROR != pInstance->Ready(min, max, shaderID))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
