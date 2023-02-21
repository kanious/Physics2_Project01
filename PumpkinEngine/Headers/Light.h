#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Base.h"
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

NAMESPACE_BEGIN(Engine)

// Light Info class
class ENGINE_API CLight : public CBase
{
public:
    enum eLightType
    {
        DIRECTIONAL_LIGHT = 0,
        POINT_LIGHT = 1,
        SPOT_LIGHT = 2
    };
    class cLightInfo
    {
    public:
        cLightInfo()
            : position(glm::vec4(0.f, 0.f, 0.f, 1.f))
            , direction(glm::vec4(0.f, 0.f, 1.f, 1.f))
            , diffuse(glm::vec4(1.f))
            , specular(glm::vec4(1.f))
            , ambient(glm::vec4(0.f, 0.f, 0.f, 0.2f))
            , atten(glm::vec4(0.01f, 0.01f, 0.f, 1.f))
            , param1(glm::vec4(0.f, 0.f, 0.f, 1.f))
            , param2(glm::vec4(1.f, 0.f, 0.f, 0.f))
        {}
        std::string     name;
        glm::vec4       position;
        glm::vec4       direction;
        glm::vec4       diffuse;
        glm::vec4       specular;       // rgp = highlight color, w = power
        glm::vec4       ambient;        // rgp = ambient color, w = power
        glm::vec4       atten;          // x = constant, y = linear, z = quadratic, w = cutoff range
        glm::vec4       param1;         // x = lightType, y = inner angle(Theta), z = outer angle(Phi), w = TBD(?)
        glm::vec4       param2;         // x = turn (1:on, 0:off)
    };

private:
    cLightInfo*         m_pInfo;
    _uint               m_ShaderProgram;
    _uint               m_shaderLocation[8];

private:
	explicit CLight();
	virtual ~CLight();
	void Destroy();

public:
    cLightInfo* GetLightInfo()                  { return m_pInfo; }
    _uint GetLocation(_int index);
    void SetLocation(_int index, _uint value);
    void SetPosition(glm::vec3 vPos);
    void SetDirection(glm::vec3 vDir);
    void SetDiffuseColor(glm::vec4 vColor);
    void SetSpecularColor(glm::vec4 vColor);
    void SetAmbientColor(glm::vec4 vColor);
    void SetAttenuation_Constant(_float value);
    void SetAttenuation_Linear(_float value);
    void SetAttenuation_Quadratic(_float value);
    void SetLightType(eLightType type);
    void SetInnerAngle(_float value);
    void SetOuterAngle(_float value);
    void SetShaderProgram(_uint shaderID)       { m_ShaderProgram = shaderID; }
    void TurnOnOff(_bool onoff);
public:
    void ResetLightInfo();

private:
    RESULT Ready(cLightInfo* pInfo);
public:
    static CLight* Create(cLightInfo* pInfo);
};

NAMESPACE_END

#endif //_LIGHT_H_