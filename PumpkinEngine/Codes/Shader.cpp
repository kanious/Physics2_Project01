#include "pch.h"
#include "../Headers/Shader.h"
#include "../Headers/Component.h"
#include <iostream>
#include <fstream>
#include "../Headers/OpenGLDefines.h"


USING(Engine)
USING(std)
USING(glm)

CShader::CShader()
	: m_ShaderProgram(0)
	, m_matWorldLocation(0)
	, m_matViewLocation(0)
	, m_matProjLocation(0)
	, m_diffTexLocation(0)
	, m_normalTexLocation(0)
	, m_lightEnableLocation(0)
	, m_selectedLocation(0)
	, m_colorLocation(0)
	, m_transparencyLocation(0)
	, m_frameLocation(0)
{
}

CShader::CShader(const CShader& rhs)
	: m_ShaderProgram(rhs.m_ShaderProgram)
	, m_matWorldLocation(rhs.m_matWorldLocation)
	, m_matViewLocation(rhs.m_matViewLocation)
	, m_matProjLocation(rhs.m_matProjLocation)
	, m_diffTexLocation(rhs.m_diffTexLocation)
	, m_normalTexLocation(rhs.m_normalTexLocation)
	, m_lightEnableLocation(rhs.m_lightEnableLocation)
	, m_selectedLocation(rhs.m_selectedLocation)
	, m_colorLocation(rhs.m_colorLocation)
	, m_transparencyLocation(rhs.m_transparencyLocation)
	, m_frameLocation(rhs.m_frameLocation)
{
	m_tag = rhs.m_tag;
}

CShader::~CShader()
{
}

// Call instead of destructor to manage class internal data
void CShader::Destroy()
{
	CComponent::Destroy();
}

// Create shader Id
RESULT CShader::CreateShaderProgram()
{
	m_ShaderProgram = glCreateProgram();

	return PK_NOERROR;
}

// Create new shader from file
RESULT CShader::AddShader(const char* vertexPath, const char* fragPath)
{
	string vertex_shader_code = ReadShader(vertexPath);
	string fragment_shader_code = ReadShader(fragPath);
	_uint vertexShader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code);
	_uint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code);

	int link_result = 0;
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
		return PK_SHADER_CREATE_FAILED;

	glUseProgram(m_ShaderProgram);

	SetLocation();

	return PK_NOERROR;
}

// Read shader code from file
string CShader::ReadShader(const char* fileName)
{
	string shaderCode;
	ifstream file(fileName, std::ios::in);

	if (!file.good())
	{
		terminate();
		return "";
	}

	file.seekg(0, ios::end);
	shaderCode.resize((_uint)file.tellg());
	file.seekg(0, ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

// Compile shader
_uint CShader::CreateShader(_uint shaderType, string source)
{
	_int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char* shader_code_ptr = source.c_str();
	const _int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE)
		return 0;

	return shader;
}

// Set location to shader
void CShader::SetLocation()
{
 	m_matWorldLocation = glGetUniformLocation(m_ShaderProgram, "matWorld");
	m_matViewLocation = glGetUniformLocation(m_ShaderProgram, "matView");
	m_matProjLocation = glGetUniformLocation(m_ShaderProgram, "matProj");
	m_diffTexLocation = glGetUniformLocation(m_ShaderProgram, "diffTexture");
	m_normalTexLocation = glGetUniformLocation(m_ShaderProgram, "normalTexture");
	m_lightEnableLocation = glGetUniformLocation(m_ShaderProgram, "isLightEnable");
	m_selectedLocation = glGetUniformLocation(m_ShaderProgram, "isSelected");
	m_colorLocation = glGetUniformLocation(m_ShaderProgram, "vColor");
	m_transparencyLocation = glGetUniformLocation(m_ShaderProgram, "isTransparency");
	m_frameLocation = glGetUniformLocation(m_ShaderProgram, "iFrameIndex");
 }

// Set matrix information to shader
void CShader::SetMatrixInfo(const mat4x4 world, const mat4x4 view, const mat4x4 proj)
{
	glUseProgram(m_ShaderProgram);
	glUniformMatrix4fv(m_matWorldLocation, 1, GL_FALSE, value_ptr(world));
	glUniformMatrix4fv(m_matViewLocation, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(m_matProjLocation, 1, GL_FALSE, value_ptr(proj));
}

// Set diffuse texture information to shader
void CShader::SetTextureInfo()
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_diffTexLocation, 0);
}

// Set normal texture information to shader
void CShader::SetNormalTextureInfo()
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_normalTexLocation, 1);
}

// Set light enable information to shader
void CShader::SetLightEnableInfo(_bool lightEnable)
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_lightEnableLocation, lightEnable);
}

// Set selected object information to shader
void CShader::SetSelected(_bool selected)
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_selectedLocation, selected);
}

// Set mesh color information to shader
void CShader::SetColor(vec3 vColor)
{
	glUseProgram(m_ShaderProgram);
	glUniform4f(m_colorLocation, vColor.x, vColor.y, vColor.z, 1.f);
}

// Set mesh transparency information to shader
void CShader::SetTransparency(_bool value)
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_transparencyLocation, value);
}

// Set animation frame index information to shader
void CShader::SetFrameIndex(_uint index)
{
	glUseProgram(m_ShaderProgram);
	glUniform1i(m_frameLocation, index);
}

// Initialize Shader
RESULT CShader::Ready(string ID, const char* vertexPath, const char* fragPath)
{
	m_tag = ID;

	RESULT result;
	result = CreateShaderProgram();
	if (PK_NOERROR != result)
		return result;

	result = AddShader(vertexPath, fragPath);
	if (PK_NOERROR != result)
		return result;

	return PK_NOERROR;
}

// Clone component
CComponent* CShader::Clone()
{
	return new CShader(*this);
}

// Create an instance
CShader* CShader::Create(string ID, const char* vertexPath, const char* fragPath)
{
	CShader* pInstance = new CShader;
	if (PK_NOERROR != pInstance->Ready(ID, vertexPath, fragPath))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}