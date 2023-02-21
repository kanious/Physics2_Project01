#ifndef _CHANNELGROUPINFO_H_
#define _CHANNELGROUPINFO_H_

#include "Base.h"
#include "fmod\fmod.hpp"
#include "fmod\fmod_dsp.h"
#include "fmod\fmod_errors.h"

NAMESPACE_BEGIN(Engine)

// Class containing FMOD channel group information
class ENGINE_API CChannelGroupInfo : public CBase
{
public:
	FMOD::ChannelGroup*			pChannelGroup;
	std::string					tag;
	_float						volume;
	_float						pitch;
	_bool						arrDspCheck[FMOD_DSP_TYPE_MAX];

public:
	explicit CChannelGroupInfo();
	virtual ~CChannelGroupInfo();
	void Destroy();

public:
	_float GetVolume();
	_float GetPitch();
	_bool GetDSP(FMOD_DSP_TYPE type);
public:
	void SetVolume(_float value);
	void SetPitch(_float value);
	void SetDSP(FMOD::DSP* pDSP, FMOD_DSP_TYPE type, _bool isOn);
	void SetDspCheckAll(_bool isOn);
	
};

NAMESPACE_END

#endif //_CHANNELGROUPINFO_H_