#ifndef _BASE_H_
#define _BASE_H_

#include "EngineDefines.h"

NAMESPACE_BEGIN(Engine)

// Super class for all (managing memory by using reference count)
class ENGINE_API CBase
{
private:
	_uint				m_iRefCnt;

protected:
	explicit CBase();
	virtual ~CBase();

public:
	// Add Reference Count
	_uint AddRefCnt();
	// Delete itself if the reference count is zero
	_uint Release();

protected:
	virtual void Destroy() = 0; //Call instead of destructor to manage class internal data
};

NAMESPACE_END

#endif //_BASE_H_