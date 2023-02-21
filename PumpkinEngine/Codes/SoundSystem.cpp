#include "pch.h"
#include "..\Headers\SoundSystem.h"
#include "..\Headers\SoundInfo.h"
#include "..\Headers\ChannelGroupInfo.h"
#include "..\Headers\DSPInfo.h"
#include <sstream>
#include <iomanip>


USING(Engine)
USING(std)
USING(FMOD)

CSoundSystem::CSoundSystem()
	: m_pSystem(nullptr)
{
	m_mapSounds.clear();
	m_mapChannels.clear();
	m_mapDSPs.clear();
}

CSoundSystem::~CSoundSystem()
{
}

// Call instead of destructor to manage class internal data
void CSoundSystem::Destroy()
{
	CHANNEL_MAP::iterator iterChannel;
	for (iterChannel = m_mapChannels.begin(); iterChannel != m_mapChannels.end(); ++iterChannel)
		SafeDestroy(iterChannel->second);
	m_mapChannels.clear();

	SOUND_MAP::iterator iterSound;
	for (iterSound = m_mapSounds.begin(); iterSound != m_mapSounds.end(); ++iterSound)
		SafeDestroy(iterSound->second);
	m_mapSounds.clear();

	DSP_MAP::iterator iterDSP;
	for (iterDSP = m_mapDSPs.begin(); iterDSP != m_mapDSPs.end(); ++iterDSP)
		SafeDestroy(iterDSP->second);
	m_mapDSPs.clear();

	if (nullptr != m_pSystem)
	{
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}

void CSoundSystem::SetListener(_float x, _float y, _float z)
{
	FMOD_VECTOR vPos;
	vPos.x = x;
	vPos.y = y;
	vPos.z = z;

	m_pSystem->set3DListenerAttributes(0, &vPos, nullptr, nullptr, nullptr);
	m_pSystem->update();
}

void CSoundSystem::SetVolume(string tag, _float volume)
{
	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
		return;

	iterSound->second->SetVolume(volume);
}

void CSoundSystem::Set3DSoundPosition(string tag, _float x, _float y, _float z)
{
	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
		return;

	if (nullptr == iterSound->second->pChannel)
		return;

	FMOD_VECTOR vSoundPos;
	vSoundPos.x = x;
	vSoundPos.y = y;
	vSoundPos.z = z;
	iterSound->second->pChannel->set3DAttributes(&vSoundPos, nullptr);
}

// Load sound from file
RESULT CSoundSystem::LoadSound(string tag, string path, string channelTag, _int mode)
{
	Sound* pSound;
	FMOD_RESULT result = m_pSystem->createSound(path.c_str(), mode, nullptr, &pSound);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_SOUND_CREATE_ERROR;

	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
	{
		CSoundInfo* pInfo = new CSoundInfo();
		pInfo->pSound = pSound;
		FMOD_RESULT result;

		// tag
		pInfo->tag = tag;
		pInfo->channelGroupTag = channelTag;
		// name
		result = pSound->getName(pInfo->name, sizeof(pInfo->name));
		// maxLength
		result = pSound->getLength(&pInfo->maxLength, FMOD_TIMEUNIT_MS);
		_uint minute = pInfo->maxLength / 1000 / 60;
		_uint second = pInfo->maxLength / 1000 % 60;
		stringstream ss;
		ss << setw(2) << setfill('0') << minute;
		ss << ":";
		ss << setw(2) << setfill('0') << second;
		pInfo->strMaxLength = ss.str();

		// type, format
		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		result = pSound->getFormat(&type, &format, &pInfo->channels, &pInfo->bits);
		pInfo->SetSoundType(type);
		pInfo->SetSoundFormat(format);

		// loop
		if (FMOD_LOOP_NORMAL == mode)
			pInfo->loop = true;
		else
			pInfo->loop = false;

		m_mapSounds.insert(SOUND_MAP::value_type(tag, pInfo));
	}
	else
	{
		pSound->release();
		return PK_FMOD_SOUND_EXIST;
	}

	return PK_NOERROR;
}

// Create channel group
RESULT CSoundSystem::CreateChannelGroup(std::string tag)
{
	ChannelGroup* pChannelGroup;
	FMOD_RESULT result = m_pSystem->createChannelGroup(tag.c_str(), &pChannelGroup);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_CHANNEL_CREATE_ERROR;

	CHANNEL_MAP::iterator iterChannel = m_mapChannels.find(tag);
	if (iterChannel == m_mapChannels.end())
	{
		CChannelGroupInfo* pInfo = new CChannelGroupInfo();
		pInfo->pChannelGroup = pChannelGroup;
		pInfo->tag = tag;
		m_mapChannels.insert(CHANNEL_MAP::value_type(tag, pInfo));
	}
	else
	{
		pChannelGroup->release();
		return PK_FMOD_CHANNEL_EXIST;
	}

	return PK_NOERROR;
}

// Play sound
void CSoundSystem::PlaySound(string tag)
{
	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
		return;

	CSoundInfo* pInfo = iterSound->second;
	if (nullptr == pInfo)
		return;

	CHANNEL_MAP::iterator iterChannel = m_mapChannels.find(pInfo->channelGroupTag);
	if (iterChannel == m_mapChannels.end())
		return;

	if (pInfo->loop)
	{
		if (nullptr != pInfo->pChannel)
		{
			pInfo->GetSoundState();
			if (pInfo->soundState == "PLAYING")
				return;
		}
	}
	iterChannel->second->pChannelGroup->stop();

	FMOD::Channel* pChannel;
	m_pSystem->playSound(pInfo->pSound, iterChannel->second->pChannelGroup, 0, &pChannel);
	pInfo->pChannel = pChannel;
}

// Play 3D sound
void CSoundSystem::Play3DSound(string tag, _float x, _float y, _float z)
{
	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
		return;

	CSoundInfo* pInfo = iterSound->second;
	if (nullptr == pInfo)
		return;

	CHANNEL_MAP::iterator iterChannel = m_mapChannels.find(pInfo->channelGroupTag);
	if (iterChannel == m_mapChannels.end())
		return;

	if (pInfo->loop)
	{
		if (nullptr != pInfo->pChannel)
		{
			pInfo->GetSoundState();
			if (pInfo->soundState == "PLAYING")
				return;
		}
	}
 	iterChannel->second->pChannelGroup->stop();

	FMOD::Channel* pChannel;
	FMOD_RESULT result = m_pSystem->playSound(pInfo->pSound, iterChannel->second->pChannelGroup, 0, &pChannel);
	if (PK_NOERROR != ErrorCheck(result))
		return;

	pInfo->pChannel = pChannel;

	FMOD_VECTOR vSoundPos;
	vSoundPos.x = x;
	vSoundPos.y = y;
	vSoundPos.z = z;
	result = pChannel->set3DAttributes(&vSoundPos, nullptr);
	if (PK_NOERROR != ErrorCheck(result))
		return;

	result = pChannel->set3DMinMaxDistance(1.0f, 10000);

	//if ("ConstructionSite" == tag || "Helicopter" == tag)
	//	result = pChannel->set3DMinMaxDistance(10.0f, 10000);
	//else
	//	result = pChannel->set3DMinMaxDistance(3.0f, 10000);
	if (PK_NOERROR != ErrorCheck(result))
		return;

}

// Stop sound
void CSoundSystem::StopSound(string tag)
{
	SOUND_MAP::iterator iterSound = m_mapSounds.find(tag);
	if (iterSound == m_mapSounds.end())
		return;

	CSoundInfo* pInfo = iterSound->second;
	if (nullptr != pInfo->pChannel)
	{
		pInfo->pChannel->stop();
		pInfo->pChannel = nullptr;
		pInfo->pan = 0.f;
	}
}

// Create DSP effect
RESULT CSoundSystem::CreateDSPEffect(string name, FMOD_DSP_TYPE type)
{
	DSP* pDSP;

	FMOD_RESULT result = m_pSystem->createDSPByType(type, &pDSP);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_DSP_ERROR;

	int num = 0;
	pDSP->getNumParameters(&num);
	for (int i = 0; i < num; ++i)
	{
		FMOD_DSP_PARAMETER_DESC* desc;
		result = pDSP->getParameterInfo(i, &desc);
		if (FMOD_OK != result)
			continue;

		switch (desc->type)
		{
		case FMOD_DSP_PARAMETER_TYPE_FLOAT:
			pDSP->setParameterFloat(i, desc->floatdesc.defaultval);
			break;
		case FMOD_DSP_PARAMETER_TYPE_INT:
			pDSP->setParameterInt(i, desc->intdesc.defaultval);
			break;
		case FMOD_DSP_PARAMETER_TYPE_BOOL:
			pDSP->setParameterBool(i, desc->booldesc.defaultval);
			break;
		case FMOD_DSP_PARAMETER_TYPE_DATA:
			break;
		}
	}

	DSP_MAP::iterator iterDSP = m_mapDSPs.find(name);
	if (iterDSP == m_mapDSPs.end())
	{
		CDSPInfo* pInfo = new CDSPInfo();
		pInfo->pDSP = pDSP;
		pInfo->tag = name;
		pInfo->type = type;
		m_mapDSPs.insert(DSP_MAP::value_type(name, pInfo));
	}
	else
	{
		pDSP->release();
		return PK_FMOD_DSP_EXIST;
	}

	return PK_NOERROR;
}

// Add DSP effect
RESULT CSoundSystem::AddDSPEffect(string channelTag, string dspName)
{
	CHANNEL_MAP::iterator iterChannel = m_mapChannels.find(channelTag);
	if (iterChannel == m_mapChannels.end())
		return PK_FMOD_CHANNEL_NOT_EXIST;

	DSP_MAP::iterator iterDSP = m_mapDSPs.find(dspName);
	if (iterDSP == m_mapDSPs.end())
		return PK_FMOD_DSP_NOT_EXIST;

	int num;
	FMOD_RESULT result = iterChannel->second->pChannelGroup->getNumDSPs(&num);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_DSP_ERROR;

	result = iterChannel->second->pChannelGroup->addDSP(num, iterDSP->second->pDSP);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_DSP_ERROR;

	return PK_NOERROR;
}

// FMOD Error Check
RESULT CSoundSystem::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("fmod error: %d, %s\n", result, FMOD_ErrorString(result));
		return PK_FMOD_ERROR;
	}

	return PK_NOERROR;
}

// Initialize
RESULT CSoundSystem::Ready(_int number, _int flag)
{
	FMOD_RESULT result = System_Create(&m_pSystem);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_ERROR;

	result = m_pSystem->init(number, flag, nullptr);
	if (PK_NOERROR != ErrorCheck(result))
		return PK_FMOD_ERROR;

	return PK_NOERROR;
}

// Create an instance
CSoundSystem* CSoundSystem::Create(_int number, _int flag)
{
	CSoundSystem* pInstance = new CSoundSystem();
	if (PK_NOERROR != pInstance->Ready(number, flag))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}
