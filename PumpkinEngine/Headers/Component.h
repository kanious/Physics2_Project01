#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

//Parent class of classes with various components
class ENGINE_API CComponent : public CBase
{
protected:
	std::string				m_tag;
	_bool					m_bEnable;

protected:
	explicit CComponent();
	virtual ~CComponent();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
	void Enable();
	void Disable();
protected:
	virtual void Destroy();

public:
	std::string GetTag()				{ return m_tag; }
	const _bool GetEnable()				{ return m_bEnable; }
	void SetTag(std::string componentTag);
	void SetEnable(_bool enable)		{ m_bEnable = enable; }

public:
	virtual CComponent* Clone() = 0;
};

NAMESPACE_END

#endif //_COMPONENT_H_