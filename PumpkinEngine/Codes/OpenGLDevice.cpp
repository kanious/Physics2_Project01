#include "pch.h"
#include "..\Headers\OpenGLDevice.h"


USING(Engine)
USING(glm)
SINGLETON_FUNCTION(COpenGLDevice)

COpenGLDevice::COpenGLDevice()
{
	m_pWindow = nullptr;
	m_iWidth = 800;
	m_iHeight = 600;
	m_matView = mat4x4(1.f);
	m_matProj = mat4x4(1.f);
}

COpenGLDevice::~COpenGLDevice()
{
}

// Call instead of destructor to manage class internal data
void COpenGLDevice::Destroy()
{
	TerminateOpenGLWindow();
}

void COpenGLDevice::GetWindowSize()
{
	glfwGetWindowSize(m_pWindow, &m_iWidth, &m_iHeight);
}

void COpenGLDevice::SetWindowSize(_int width, _int height)
{
	glfwDestroyWindow(m_pWindow);
	m_pWindow = glfwCreateWindow(width, height, m_title, NULL, NULL);
	if (!m_pWindow)
		glfwTerminate();

	glfwMakeContextCurrent(m_pWindow);
}

void COpenGLDevice::SetViewMatrix(mat4x4 matView)
{
	m_matView = matView;
}

void COpenGLDevice::SetProjMatrix(mat4x4 matProj) 
{
	m_matProj = matProj;
}

// Create OpenGL Window
RESULT COpenGLDevice::CreateOpenGLWindow(_int width, _int height, const char* title, _bool fullscreen, _bool borderedWindow)
{
	if (!glfwInit())
		return PK_GLFW_INIT_FAIL;

	glfwDefaultWindowHints();
	// GLAD 레거시 코드를 사용하려면 glfw창 생성 전에 버전 정보 필요
	// 내가 다운받은 버전으로
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	if (fullscreen)
	{
		m_pWindow = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
		if (!m_pWindow) // means if window is 0, not allocated
		{
			glfwTerminate();
			return PK_WINDOW_CREATE_ERROR;
		}
	}
	else if (borderedWindow)
	{
		m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!m_pWindow) // means if window is 0, not allocated
		{
			glfwTerminate();
			return PK_WINDOW_CREATE_ERROR;
		}
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(m_pWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwSetWindowAttrib(m_pWindow, GLFW_DECORATED, GLFW_FALSE);
	}
	else
	{
		m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!m_pWindow) // means if window is 0, not allocated
		{
			glfwTerminate();
			return PK_WINDOW_CREATE_ERROR;
		}
		glfwMaximizeWindow(m_pWindow);
		glfwSetWindowAttrib(m_pWindow, GLFW_DECORATED, GLFW_TRUE);
	}
	
	m_title = title;
	m_iWidth = width;
	m_iHeight = height;

	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = GL_TRUE;
	glewInit();
	
	glCullFace(GL_BACK);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST | GL_BLEND);
	//glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	return PK_NOERROR;
}

// Terminate OpenGL Window
void COpenGLDevice::TerminateOpenGLWindow()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}