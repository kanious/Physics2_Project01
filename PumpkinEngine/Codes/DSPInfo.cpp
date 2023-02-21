#include "pch.h"
#include "..\Headers\DSPInfo.h"


USING(Engine)
USING(std)
USING(FMOD)

CDSPInfo::CDSPInfo()
	: pDSP(nullptr), tag(""), type(FMOD_DSP_TYPE_UNKNOWN)
{
}

CDSPInfo::~CDSPInfo()
{
}

void CDSPInfo::Destroy()
{
	if (nullptr != pDSP)
		pDSP->release();
}

_int CDSPInfo::GetParameterNum()
{
	if (nullptr == pDSP)
		return 0;

	int num = 0;
	pDSP->getNumParameters(&num);

	return  num;
}

FMOD_DSP_PARAMETER_DESC* CDSPInfo::GetParameterInfo(_int index)
{
	if (nullptr == pDSP)
		return nullptr;

	FMOD_DSP_PARAMETER_DESC* desc = nullptr;
	pDSP->getParameterInfo(index, &desc);

	return desc;
}

_bool CDSPInfo::GetDSPBoolValue(_int index)
{
	if (nullptr == pDSP)
		return false;

	_bool value = false;
	pDSP->getParameterBool(index, &value, NULL, 0);

	return value;
}

_float CDSPInfo::GetDSPFloatValue(_int index)
{
	if (nullptr == pDSP)
		return false;

	_float value = false;
	pDSP->getParameterFloat(index, &value, NULL, 0);

	return value;
}

_int CDSPInfo::GetDSPIntValue(_int index)
{
	if (nullptr == pDSP)
		return false;

	_int value = false;
	pDSP->getParameterInt(index, &value, NULL, 0);

	return value;
}

void CDSPInfo::SetDSPBoolValue(_int index, _bool value)
{
	if (nullptr == pDSP)
		return;

	pDSP->setParameterBool(index, value);
}

void CDSPInfo::SetDSPFloatValue(_int index, _float value)
{
	if (nullptr == pDSP)
		return;

	pDSP->setParameterFloat(index, value);
}

void CDSPInfo::SetDSPIntValue(_int index, _int value)
{
	if (nullptr == pDSP)
		return;

	pDSP->setParameterInt(index, value);
}

void CDSPInfo::SetDefaultValue()
{
	if (nullptr == pDSP)
		return;

	_int num = 0;
	pDSP->getNumParameters(&num);
	FMOD_RESULT result;
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
}
