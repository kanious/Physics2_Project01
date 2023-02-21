#include "pch.h"
#include "..\Headers\ChannelGroupInfo.h"


USING(Engine)
USING(std)
USING(FMOD)

CChannelGroupInfo::CChannelGroupInfo()
	: pChannelGroup(nullptr)
{
	ZeroMemory(arrDspCheck, sizeof(*arrDspCheck));
}

CChannelGroupInfo::~CChannelGroupInfo()
{
}

void CChannelGroupInfo::Destroy()
{
	if (nullptr != pChannelGroup)
		pChannelGroup->release();
}

_float CChannelGroupInfo::GetVolume()
{
	if (nullptr == pChannelGroup)
		return 0.f;

	pChannelGroup->getVolume(&volume);
	return volume;
}

_float CChannelGroupInfo::GetPitch()
{
	if (nullptr == pChannelGroup)
		return 0.f;

	pChannelGroup->getPitch(&pitch);
	return pitch;
}

_bool CChannelGroupInfo::GetDSP(FMOD_DSP_TYPE type)
{
	if (nullptr == arrDspCheck)
		return false;

	return arrDspCheck[type];
}

void CChannelGroupInfo::SetVolume(_float value)
{
	if (nullptr == pChannelGroup)
		return;

	pChannelGroup->setVolume(value);
}

void CChannelGroupInfo::SetPitch(_float value)
{
	if (nullptr == pChannelGroup)
		return;

	pChannelGroup->setPitch(value);
}

void CChannelGroupInfo::SetDSP(DSP* pDSP, FMOD_DSP_TYPE type, _bool isOn)
{
	if (nullptr == pDSP)
		return;

	if (arrDspCheck[type] == isOn)
		return;

	FMOD_RESULT result;
	if (isOn)
	{
		int index = 0;
		result = pChannelGroup->getNumDSPs(&index);
		if (FMOD_OK != result) return;
		result = pChannelGroup->addDSP(index, pDSP);
		if (FMOD_OK != result) return;
	}
	else
	{
		result = pChannelGroup->removeDSP(pDSP);
		if (FMOD_OK != result) return;
	}
	arrDspCheck[type] = isOn;
}

void CChannelGroupInfo::SetDspCheckAll(_bool isOn)
{
	memset(arrDspCheck, (_int)isOn, sizeof(*arrDspCheck));
}
