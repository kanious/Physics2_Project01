#include "pch.h"
#include "..\Headers\SoundInfo.h"


USING(Engine)
USING(std)
USING(FMOD)

CSoundInfo::CSoundInfo()
	: pSound(nullptr), pChannel(nullptr), tag(""), maxLength(0), strMaxLength("")
	, soundType(""), soundFormat(""), channels(0), bits(0), loop(false), channelGroupTag("")
	, soundState(""), currentPosition(0), speed(0.f), volume(0.f), pitch(0.f)
{
	ZeroMemory(name, sizeof(name));
}

CSoundInfo::~CSoundInfo()
{
}

void CSoundInfo::Destroy()
{
	if (nullptr != pSound)
		pSound->release();
}

std::string CSoundInfo::GetSoundState()
{
	if (nullptr == pSound)
		return "";

	FMOD_OPENSTATE openstate;
	unsigned int percentbuffered; bool starving, diskbusy;
	pSound->getOpenState(&openstate, &percentbuffered, &starving, &diskbusy);
	soundState = GetSoundState(openstate);

	return soundState;
}

_uint CSoundInfo::GetCurrentPosition()
{
	if (nullptr == pChannel)
		return 0;

	pChannel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
	return currentPosition;
}

_float CSoundInfo::GetFrequency()
{
	if (nullptr == pChannel)
		return 0.f;

	pChannel->getFrequency(&speed);
	return speed;
}

_float CSoundInfo::GetVolumn()
{
	if (nullptr == pChannel)
		return 0.f;

	pChannel->getVolume(&volume);
	return volume;
}

_float CSoundInfo::GetPitch()
{
	if (nullptr == pChannel)
		return 0.f;

	pChannel->getPitch(&pitch);
	return pitch;
}

void CSoundInfo::SetSoundType(FMOD_SOUND_TYPE type)
{
	soundType = GetSoundType(type);
}

void CSoundInfo::SetSoundFormat(FMOD_SOUND_FORMAT format)
{
	soundFormat = GetSoundFormat(format);
}

void CSoundInfo::SetCurrentPosition(_uint value)
{
	if (nullptr == pChannel)
		return;

	pChannel->setPosition(value, FMOD_TIMEUNIT_MS);
}

void CSoundInfo::SetFrequency(_float value)
{
	if (nullptr == pChannel)
		return;

	pChannel->setFrequency(value);
}

void CSoundInfo::SetVolume(_float value)
{
	if (nullptr == pChannel)
		return;

	pChannel->setVolume(value);
}

void CSoundInfo::SetPitch(_float value)
{
	if (nullptr == pChannel)
		return;

	pChannel->setPitch(value);
}

void CSoundInfo::SetPan(_float value)
{
	if (nullptr == pChannel)
		return;

	pan = value;
	pChannel->setPan(value);
}

string CSoundInfo::GetSoundState(FMOD_OPENSTATE state)
{
	string str = "";

	switch (state)
	{
	case FMOD_OPENSTATE_READY:				str = "READY";					break;
	case FMOD_OPENSTATE_LOADING:			str = "LOADING";				break;
	case FMOD_OPENSTATE_ERROR:				str = "ERROR";					break;
	case FMOD_OPENSTATE_CONNECTING:			str = "CONNECTING";				break;
	case FMOD_OPENSTATE_BUFFERING:			str = "BUFFERING";				break;
	case FMOD_OPENSTATE_SEEKING:			str = "SEEKING";				break;
	case FMOD_OPENSTATE_PLAYING:			str = "PLAYING";				break;
	case FMOD_OPENSTATE_SETPOSITION:		str = "SETPOSITION";			break;
	case FMOD_OPENSTATE_MAX:				str = "MAX";					break;
	case FMOD_OPENSTATE_FORCEINT:			str = "FORCEINT";				break;
	}

	return str;
}
string CSoundInfo::GetSoundType(FMOD_SOUND_TYPE type)
{
	string str = "";

	switch (type)
	{
	case FMOD_SOUND_TYPE_UNKNOWN:			str = "UNKNOWN";				 break;
	case FMOD_SOUND_TYPE_AIFF:				str = "AIFF ";					 break;
	case FMOD_SOUND_TYPE_ASF:				str = "ASF";					 break;
	case FMOD_SOUND_TYPE_DLS:				str = "DLS";					 break;
	case FMOD_SOUND_TYPE_FLAC:				str = "FLAC";					 break;
	case FMOD_SOUND_TYPE_FSB:				str = "FSB";					 break;
	case FMOD_SOUND_TYPE_IT:				str = "IT";						 break;
	case FMOD_SOUND_TYPE_MIDI:				str = "MIDI";					 break;
	case FMOD_SOUND_TYPE_MOD:				str = "MOD";					 break;
	case FMOD_SOUND_TYPE_MPEG:				str = "MPEG";					 break;
	case FMOD_SOUND_TYPE_OGGVORBIS:			str = "OGGVORBIS";				 break;
	case FMOD_SOUND_TYPE_PLAYLIST:			str = "PLAYLIST";				 break;
	case FMOD_SOUND_TYPE_RAW:				str = "RAW";					 break;
	case FMOD_SOUND_TYPE_S3M:				str = "S3M";					 break;
	case FMOD_SOUND_TYPE_USER:				str = "USER";					 break;
	case FMOD_SOUND_TYPE_WAV:				str = "WAV";					 break;
	case FMOD_SOUND_TYPE_XM:				str = "XM";						 break;
	case FMOD_SOUND_TYPE_XMA:				str = "XMA";					 break;
	case FMOD_SOUND_TYPE_AUDIOQUEUE:		str = "AUDIOQUEUE";				 break;
	case FMOD_SOUND_TYPE_AT9:				str = "AT9";					 break;
	case FMOD_SOUND_TYPE_VORBIS:			str = "VORBIS";					 break;
	case FMOD_SOUND_TYPE_MEDIA_FOUNDATION:	str = "MEDIA_FOUNDATION";		 break;
	case FMOD_SOUND_TYPE_MEDIACODEC:		str = "MEDIACODEC";				 break;
	case FMOD_SOUND_TYPE_FADPCM:			str = "FADPCM";					 break;
	case FMOD_SOUND_TYPE_OPUS:				str = "OPUS";					 break;
	case FMOD_SOUND_TYPE_MAX:				str = "MAX";					 break;
	case FMOD_SOUND_TYPE_FORCEINT:			str = "FORCEINT";				 break;
	}
	return str;
}

string CSoundInfo::GetSoundFormat(FMOD_SOUND_FORMAT format)
{
	string str = "";

	switch (format)
	{
	case FMOD_SOUND_FORMAT_NONE:			str = "NONE";					break;
	case FMOD_SOUND_FORMAT_PCM8:			str = "PCM8";					break;
	case FMOD_SOUND_FORMAT_PCM16:			str = "PCM16";					break;
	case FMOD_SOUND_FORMAT_PCM24:			str = "PCM24";					break;
	case FMOD_SOUND_FORMAT_PCM32:			str = "PCM32";					break;
	case FMOD_SOUND_FORMAT_PCMFLOAT:		str = "PCMFLOAT";				break;
	case FMOD_SOUND_FORMAT_BITSTREAM:		str = "BITSTREAM";				break;
	case FMOD_SOUND_FORMAT_MAX:				str = "MAX";					break;
	case FMOD_SOUND_FORMAT_FORCEINT:		str = "FORCEINT";				break;
	}

	return str;
}