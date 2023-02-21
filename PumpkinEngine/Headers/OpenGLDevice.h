#ifndef _OPENGLDEVICCE_H_
#define _OPENGLDEVICCE_H_

#include "Base.h"
#include "OpenGLDefines.h"

NAMESPACE_BEGIN(Engine)

// OpenGL Graphics device
class ENGINE_API COpenGLDevice : public CBase
{
	SINGLETON(COpenGLDevice)

private:
	GLFWwindow*					m_pWindow;
	const char*					m_title;
	_int						m_iWidth;
	_int						m_iHeight;
	glm::mat4x4					m_matView;
	glm::mat4x4					m_matProj;

private:
	explicit COpenGLDevice();
	virtual ~COpenGLDevice();
	// Call instead of destructor to manage class internal data
	void Destroy();

public:
	GLFWwindow* GetWindow()						{ return m_pWindow; }
	const glm::mat4x4 GetViewMatrix()			{ return m_matView; }
	const glm::mat4x4 GetProjMatrix()			{ return m_matProj; }
	const _int GetWidthSize()					{ return m_iWidth; }
	const _int GetHeightSize()					{ return m_iHeight; }
	void GetWindowSize();
	void SetWindowSize(_int width, _int height);
	void SetViewMatrix(glm::mat4x4 matView);
	void SetProjMatrix(glm::mat4x4 matProj);
public:
	// Create OpenGL Window
	RESULT CreateOpenGLWindow(_int width, _int height, const char* title, _bool fullscreen, _bool borderedWindow);
private:
	// Terminate OpenGL Window
	void TerminateOpenGLWindow();

};

NAMESPACE_END

#endif //_OPENGLDEVICCE_H_