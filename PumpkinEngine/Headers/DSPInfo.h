#ifndef _DSPINFO_H_
#define _DSPINFO_H_

#include "Base.h"
#include "fmod\fmod.hpp"
#include "fmod\fmod_dsp.h"
#include "fmod\fmod_errors.h"

NAMESPACE_BEGIN(Engine)

// Class containing FMOD DSP Effects information
class ENGINE_API CDSPInfo : public CBase
{
public:
	FMOD::DSP*					pDSP;
	std::string					tag;
	FMOD_DSP_TYPE				type;

public:
	explicit CDSPInfo();
	virtual ~CDSPInfo();
	void Destroy();

public:
	_int GetParameterNum();
	FMOD_DSP_PARAMETER_DESC* GetParameterInfo(_int index);
	_bool GetDSPBoolValue(_int index);
	_float GetDSPFloatValue(_int index);
	_int GetDSPIntValue(_int index);
public:
	void SetDSPBoolValue(_int index, _bool value);
	void SetDSPFloatValue(_int index, _float value);
	void SetDSPIntValue(_int index, _int value);
	void SetDefaultValue();
};

NAMESPACE_END

#endif //_DSPINFO_H_