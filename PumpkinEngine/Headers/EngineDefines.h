#ifndef _ENGINEDEFINE_H_
#define _ENGINEDEFINE_H_
#pragma warning(disable : 4251) // STL DLL Warning

// Default Headers
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <iostream>

// Custom Headers
#include "EngineError.h"

// Libraries
#pragma comment(lib, "Rpcrt4.lib")

// Define/Typedef
#ifdef PUMPKIN_ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define NAMESPACE_BEGIN(NAMESPACE) namespace NAMESPACE {
#define NAMESPACE_END }
#define USING(NAMESPACE) using namespace NAMESPACE;

#define SINGLETON(CLASSNAME)							\
		private:										\
			static CLASSNAME* m_pInstance;				\
			void DestroyInstance();						\
		public:											\
			static CLASSNAME* GetInstance();		

#define SINGLETON_FUNCTION(CLASSNAME)					\
		CLASSNAME* CLASSNAME::m_pInstance = nullptr;	\
		CLASSNAME* CLASSNAME::GetInstance()				\
		{												\
			if (nullptr == m_pInstance)					\
				m_pInstance = new CLASSNAME;			\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance()				\
		{												\
			if (nullptr != m_pInstance)					\
				delete m_pInstance;						\
				m_pInstance = nullptr;					\
		}

typedef bool				_bool;
typedef char				_char;
typedef signed short		_short;
typedef signed int			_int;
typedef signed long			_long;
typedef float				_float;
typedef double				_double;
typedef wchar_t				_wchar_t;
typedef char32_t			_char32_t;

typedef unsigned char		_uchar;
typedef unsigned short		_ushort;
typedef unsigned int		_uint;
typedef unsigned long		_ulong;

typedef unsigned int		RESULT;


template <typename T>
_uint SafeDestroy(T* pInstance)
{
	_uint iRefCnt = 0;

	if (nullptr != pInstance)
	{
		iRefCnt = pInstance->Release();
		if (0 == iRefCnt)
			pInstance = nullptr;
	}

	return iRefCnt;
}

#endif //_ENGINEDEFINE_H_