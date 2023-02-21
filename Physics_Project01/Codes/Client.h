#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "EngineDefines.h"

namespace Engine
{
	class CGameMaster;
	class CTimer;
	class COpenGLDevice;
	class CInputDevice;
}

// A client class that runs a core loop
class Client
{
private:
	Engine::CGameMaster*			m_pGameMaster;
	Engine::CTimer*					m_pTimer;
	Engine::COpenGLDevice*			m_pGraphicDevice;
	Engine::CInputDevice*			m_pInputDevice;
	_int                            m_iFPS;

	std::string						m_DataPath;
	std::string						m_SoundDataFileName;
	std::string						m_ShaderDataFileName;
	std::string						m_TextureDataFileName;
	std::string						m_MeshDataFileName;

public:
	explicit Client();
	~Client();
	void Destroy();

public:
	// Core Loop
	void Loop();
	RESULT Ready();
private:
	RESULT Ready_BasicComponent();
};

#endif //_CLIENT_H_