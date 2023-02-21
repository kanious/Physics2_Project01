#include "pch.h"
#include "..\Headers\Light.h"
#include "..\Headers\OpenGLDefines.h"

USING(Engine)

CLight::CLight()
	: m_pInfo(nullptr), m_ShaderProgram(0)
{
	ZeroMemory(m_shaderLocation, sizeof(m_shaderLocation));
}

CLight::~CLight()
{
}

// Call instead of destructor to manage class internal data
void CLight::Destroy()
{
	if (nullptr != m_pInfo)
		delete m_pInfo;
}

// Get Shader Location
_uint CLight::GetLocation(_int index)
{
	if (0 > index || 8 < index)
		return 0;

	return m_shaderLocation[index];
}

// Set Shader Location
void CLight::SetLocation(_int index, _uint value)
{
	if (0 > index || 8 < index)
		return;

	m_shaderLocation[index] = value;
}

// Reset Light Information
void CLight::ResetLightInfo()
{
	glUseProgram(m_ShaderProgram);

	for (int i = 0; i < 8; ++i)
		glUniform4f(m_shaderLocation[i], 0.f, 0.f, 0.f, 0.f);
}

// Initialize
RESULT CLight::Ready(cLightInfo* pInfo)
{
	if (nullptr == pInfo)
		return PK_ERROR_NULLPTR;

	m_pInfo = pInfo;

	return PK_NOERROR;
}

// Create an instance
CLight* CLight::Create(cLightInfo* pInfo)
{
	CLight* pInstance = new CLight();
	if (PK_NOERROR != pInstance->Ready(pInfo))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}