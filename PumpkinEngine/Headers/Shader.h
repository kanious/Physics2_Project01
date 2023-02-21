#ifndef _SHADER_H_
#define _SHADER_H_

#include "Component.h"
#include "glm/mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

// Component for shader data
class ENGINE_API CShader : public CComponent
{
protected:
	_uint				m_ShaderProgram;
	_uint				m_matWorldLocation;
	_uint				m_matViewLocation;
	_uint				m_matProjLocation;
	_uint				m_diffTexLocation;
	_uint				m_normalTexLocation;
	_uint				m_lightEnableLocation;
	_uint				m_selectedLocation;
	_uint				m_colorLocation;
	_uint				m_transparencyLocation;
	_uint				m_frameLocation;

private:
	explicit CShader();
	explicit CShader(const CShader& rhs);
	virtual ~CShader();
private:
	virtual void Destroy();

private:
	// Create shader Id
	RESULT CreateShaderProgram();
	// Create new shader from file
	RESULT AddShader(const char* vertexPath, const char* fragPath);
	// Read shader code from file
	std::string ReadShader(const char* fileName);
	// Compile shader
	_uint CreateShader(_uint shaderType, std::string source);
	// Set location to shader
	void SetLocation();
public:
	_uint GetShaderProgram()		{ return m_ShaderProgram; }
	// Set matrix information to shader
	void SetMatrixInfo(const glm::mat4x4 world, const glm::mat4x4 view, const glm::mat4x4 proj);
	// Set diffuse texture information to shader
	void SetTextureInfo();
	// Set normal texture information to shader
	void SetNormalTextureInfo();
	// Set light enable information to shader
	void SetLightEnableInfo(_bool lightEnable);
	// Set selected object information to shader
	void SetSelected(_bool selected);
	// Set mesh color information to shader
	void SetColor(glm::vec3 vColor);
	// Set mesh transparency information to shader
	void SetTransparency(_bool value);
	// Set animation frame index information to shader
	void SetFrameIndex(_uint index);

private:
	// Initialize Shader
	RESULT Ready(std::string ID, const char* vertexPath, const char* fragPath);
public:
	// Clone component
	virtual CComponent* Clone();
	// Create an instance
	static CShader* Create(std::string ID, const char* vertexPath, const char* fragPath);
};

NAMESPACE_END

#endif //_SHADER_H_