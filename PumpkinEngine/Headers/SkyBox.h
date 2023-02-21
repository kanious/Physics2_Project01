#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "GameObject.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

class COpenGLDevice;
class CShader;

// The Gameobject for skybox
class ENGINE_API CSkyBox : public CGameObject
{
private:
	COpenGLDevice*				m_pOpenGLDevice;
	CShader*					m_pShader;

	_uint						m_iTextureID;
	_uint						m_iVAO;
	_uint						m_iVBO;

private:
	explicit CSkyBox();
	virtual ~CSkyBox();

public:
	virtual void Render();
private:
	virtual void Destroy();

public:
	CShader* GetShader()							{ return m_pShader; }

private:
	RESULT Ready(std::vector<std::string>& faces, CShader* pShader);
	RESULT Ready_Texture(std::vector<std::string>& faces);
	RESULT Ready_VIBuffer();

public:
	static CSkyBox* Create(std::vector<std::string>& faces, CShader* pShader);
};

NAMESPACE_END

#endif //_SKYBOX_H_