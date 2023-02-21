#include "pch.h"
#include "..\Headers\SoundMaster.h"
#include "..\Headers\SoundSystem.h"
#include "..\Headers\SoundInfo.h"
#include "..\Headers\ChannelGroupInfo.h"


USING(Engine)
USING(std)
SINGLETON_FUNCTION(CSoundMaster)

CSoundMaster::CSoundMaster()
	: m_pSoundSystem(nullptr), m_bLoadCompressedSound(true), m_DataPath("")
{
}

CSoundMaster::~CSoundMaster()
{
}

// Call instead of destructor to manage class internal data
void CSoundMaster::Destroy()
{
	SafeDestroy(m_pSoundSystem);
}

_uint CSoundMaster::GetSoundNumber()
{
	if (nullptr == m_pSoundSystem)
		return 0;

	return m_pSoundSystem->GetSoundNumber();
}

_uint CSoundMaster::GetSoundChannelGroupNumber()
{
	if (nullptr == m_pSoundSystem)
		return 0;

	return m_pSoundSystem->GetSoundChannelGroupNumber();
}

unordered_map<string, CSoundInfo*>* CSoundMaster::GetSoundMap()
{
	if (nullptr == m_pSoundSystem)
		return nullptr;

	return m_pSoundSystem->GetSoundMap();
}

unordered_map<string, CChannelGroupInfo*>* CSoundMaster::GetChannelMap()
{
	if (nullptr == m_pSoundSystem)
		return nullptr;

	return m_pSoundSystem->GetSoundChannelMap();
}

std::unordered_map<std::string, CDSPInfo*>* CSoundMaster::GetDSPMap()
{
	if (nullptr == m_pSoundSystem)
		return nullptr;

	return m_pSoundSystem->GetDSPMap();
}

void CSoundMaster::SetListener(_float x, _float y, _float z)
{
	if (nullptr == m_pSoundSystem)
		return;

	m_pSoundSystem->SetListener(x, y, z);
}

void CSoundMaster::SetVolume(string tag, _float volume)
{
	if (nullptr == m_pSoundSystem)
		return;

	m_pSoundSystem->SetVolume(tag, volume);
}

void CSoundMaster::Set3DSoundPosition(string tag, _float x, _float y, _float z)
{
	if (nullptr == m_pSoundSystem)
		return;

	m_pSoundSystem->Set3DSoundPosition(tag, x, y, z);
}

// Load sound from file
RESULT CSoundMaster::LoadSound(string tag, string path, string channelTag)
{
	if (nullptr == m_pSoundSystem)
		return PK_ERROR_NULLPTR;

	return m_pSoundSystem->LoadSound(tag, path, channelTag, FMOD_3D | FMOD_DEFAULT);
}

// Load loop sound from file
RESULT CSoundMaster::LoadLoopSound(string tag, string path, string channelTag)
{
	if (nullptr == m_pSoundSystem)
		return PK_ERROR_NULLPTR;
	
	return m_pSoundSystem->LoadSound(tag, path, channelTag, FMOD_DEFAULT | FMOD_3D | FMOD_LOOP_NORMAL);
}

// Create channel group
RESULT CSoundMaster::CreateChannelGroup(string name)
{
	if (nullptr == m_pSoundSystem)
		return PK_ERROR_NULLPTR;

	return m_pSoundSystem->CreateChannelGroup(name);
}

// Play sound
void CSoundMaster::PlaySound(string tag)
{
	if (nullptr == m_pSoundSystem)
		return;
	
	m_pSoundSystem->PlaySound(tag);
}

// Play 3D sound
void CSoundMaster::Play3DSound(string tag, _float x, _float y, _float z)
{
	if (nullptr == m_pSoundSystem)
		return;

	m_pSoundSystem->Play3DSound(tag, x, y, z);
}

// Stop sound
void CSoundMaster::StopSound(string tag)
{
	if (nullptr != m_pSoundSystem)
		m_pSoundSystem->StopSound(tag);
}

// Create DSP effect
RESULT CSoundMaster::CreateDSPEffect(string name, _int type)
{
	if (nullptr == m_pSoundSystem)
		return PK_ERROR_NULLPTR;

	return m_pSoundSystem->CreateDSPEffect(name, (FMOD_DSP_TYPE)type);
}

// Add DSP effect
RESULT CSoundMaster::AddDSPEffect(string channelName, string dspName)
{
	if (nullptr == m_pSoundSystem)
		return PK_ERROR_NULLPTR;

	return m_pSoundSystem->AddDSPEffect(channelName, dspName);
}

// Reset all sound data
void CSoundMaster::ResetAllSoundData()
{
	SafeDestroy(m_pSoundSystem);
}

// Initialize
void CSoundMaster::Ready(_int number)
{
	m_pSoundSystem = CSoundSystem::Create(number, FMOD_INIT_NORMAL);
}
