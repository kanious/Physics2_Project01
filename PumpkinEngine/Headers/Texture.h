#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Component.h"

NAMESPACE_BEGIN(Engine)

// Component for texture data
class ENGINE_API CTexture : public CComponent
{
private:
	_uint				m_iTextureID;
	_int				m_iWidth;
	_int				m_iHeight;

private:
	explicit CTexture();
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

private:
	// Call instead of destructor to manage class internal data
	virtual void Destroy();

public:
	_uint GetTextureID()		{ return m_iTextureID; }
	_int GetWidth()				{ return m_iWidth; }
	_int GetHeight()			{ return m_iHeight; }

private:
	// Load texture information from file
	RESULT Ready(std::string ID, std::string filePath);
public:
	// Clone component
	virtual CComponent* Clone();
	// Create an instance
	static CTexture* Create(std::string ID, std::string filePath);
};

NAMESPACE_END

#endif //_TEXTURE_H_