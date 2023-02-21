#ifndef _SOUNDINFO_H_
#define _SOUNDINFO_H_

#include "Base.h"
#include "fmod\fmod.hpp"
#include "fmod\fmod_dsp.h"
#include "fmod\fmod_errors.h"

NAMESPACE_BEGIN(Engine)

// Class containing FMOD sound information
class ENGINE_API CSoundInfo : public CBase
{
public:
	// static info
	FMOD::Sound*		pSound;
	FMOD::Channel*		pChannel;
	std::string			tag;
	_char				name[128];
	_uint				maxLength;
	std::string			strMaxLength;
	std::string			soundType;
	std::string			soundFormat;
	_int				channels;
	_int				bits;
	_bool				loop;

	// dynamic info
	std::string			channelGroupTag;
	std::string			soundState;
	_uint				currentPosition;
	_float				speed;
	_float				volume;
	_float				pitch;
	_float				pan;

public:
	explicit CSoundInfo();
	virtual ~CSoundInfo();
	void Destroy();

public:
	std::string GetSoundState();
	_uint GetCurrentPosition();
	_float GetFrequency();
	_float GetVolumn();
	_float GetPitch();
	_float GetPan()			{ return pan; }
	void SetSoundType(FMOD_SOUND_TYPE type);
	void SetSoundFormat(FMOD_SOUND_FORMAT format);
	void SetCurrentPosition(_uint value);
	void SetFrequency(_float value);
	void SetVolume(_float value);
	void SetPitch(_float value);
	void SetPan(_float value);

private:
	std::string GetSoundState(FMOD_OPENSTATE state);
	std::string GetSoundType(FMOD_SOUND_TYPE type);
	std::string GetSoundFormat(FMOD_SOUND_FORMAT format);
};

NAMESPACE_END

#endif //_SOUNDINFO_H_