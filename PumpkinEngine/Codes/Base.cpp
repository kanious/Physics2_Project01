#include "pch.h"
#include "..\Headers\Base.h"

USING(Engine)
CBase::CBase()
	: m_iRefCnt(0)
{
}

CBase::~CBase()
{
}

// Add Reference Count
_uint CBase::AddRefCnt()
{
	return ++m_iRefCnt;
}

// Delete itself if the reference count is zero
_uint CBase::Release()
{
	if (m_iRefCnt == 0)
	{
		Destroy();
		delete this;
		return 0;
	}

	return m_iRefCnt--;
}
