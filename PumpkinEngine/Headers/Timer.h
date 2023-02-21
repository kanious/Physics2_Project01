#ifndef _TIMEMANAGER_H_
#define _TIMEMANAGER_H_

#include "Base.h"
#include <chrono>

NAMESPACE_BEGIN(Engine)

// Generate delta time value and FPS management
class ENGINE_API CTimer : public CBase
{
	SINGLETON(CTimer)

private:
	std::chrono::steady_clock::time_point		m_TimeLastUpdate;
	std::chrono::steady_clock::time_point		m_TimeCurrentUpdate;
	_float										m_fCurrentTime;
	_float										m_fTimeDelta;
	_float										m_fTimeDefault;
	_float										m_fFrameRate;

	_float										m_fMaxDT;

private:
	explicit CTimer();
	virtual ~CTimer();
	// Call instead of destructor to manage class internal data
	void Destroy();
public:
	// Basic Update Function, calculate default time every tick
	void Update();

public:
	_float GetTimeDelta() //{ return m_fTimeDelta > 0.1 ? 0 : m_fTimeDelta; }
	{
		if (m_fTimeDelta > m_fMaxDT)
			return m_fMaxDT;
		else
			return m_fTimeDelta;
	}
	_float GetTimeDefault() { return m_fTimeDefault; }
	// Set frame rate
	void SetFrameRate(_int frameRate);
	// Check the update cycle based on the frame rate
	_bool IsUpdateAvailable();

};

NAMESPACE_END

#endif //_TIMEMANAGER_H_