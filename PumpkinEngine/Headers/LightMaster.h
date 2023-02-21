#ifndef _LIGHTMASTER_H_
#define _LIGHTMASTER_H_

#include "Base.h"
#include "Light.h"

NAMESPACE_BEGIN(Engine)

// Class containing light information
class ENGINE_API CLightMaster : public CBase
{
	SINGLETON(CLightMaster)

private:
	std::vector<CLight*>		m_vecLights;
	_uint						m_ShaderProgram;

private:
	explicit CLightMaster();
	virtual ~CLightMaster();
	void Destroy();

public:
	std::vector<CLight*>* GetLightVector()			{ return &m_vecLights; }
public:
	RESULT AddLight(CLight::cLightInfo* pInfo);
	RESULT RemoveLight(CLight* pLight);
	void SetShader(_uint shaderID);
	void SetUniformLocation(_uint index);
	void SetLightInfo();
	void SaveLights(std::string path, std::string fileName);
	void LoadLights(std::string path, std::string fileName);
	
	void SetDirectionalLightPower(_float power);
	void TurnOnAllLightsExceptDirectional();
	void TurnOffAllLightsExceptDirectional();
	void TurnOnLight(std::string name);
	void TurnOffLight(std::string name);
	void SetLightPosition(std::string name, glm::vec3 vPos);
	void SetLightDirection(std::string name, glm::vec3 vDir);
};

//	distance	constant	linear		quadratic
//	7			1.0			0.7			1.8
//	13			1.0			0.35		0.44
//	20			1.0			0.22		0.20
//	32			1.0			0.14		0.07
//	50			1.0			0.09		0.032
//	65			1.0			0.07		0.017
//	100			1.0			0.045		0.0075
//	160			1.0			0.027		0.0028
//	200			1.0			0.022		0.0019
//	325			1.0			0.014		0.0007
//	600			1.0			0.007		0.0002
//	3250		1.0			0.0014		0.000007

NAMESPACE_END

#endif //_LIGHTMASTER_H_