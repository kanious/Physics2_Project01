#include "pch.h"
#include "..\Headers\InputDevice.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\OpenGLDevice.h"


USING(Engine)
USING(std)
USING(glm)
SINGLETON_FUNCTION(CInputDevice)

CInputDevice::CInputDevice()
{
	m_pWindow = nullptr;
	m_mapKeyInfos.clear();
	m_mapMouseButtonInfos.clear();
	m_vecMousePos = vec2(0.f);
	m_vecMousePosPrevious = vec2(0.f);
	m_vecScroll = vec2(0.f);
	m_fMouseSensitivity = 0.05f;
	m_bEntered = true;
	m_bPosFixed = false;
}

CInputDevice::~CInputDevice()
{
}

// Call instead of destructor to manage class internal data
void CInputDevice::Destroy()
{
	ClearInputSystem();
}

// Setup openGL input system
RESULT CInputDevice::SetupInputSystem(GLFWwindow* pWindow, _int mouseMode)
{
	if (nullptr == pWindow)
		return PK_INPUT_DEVICE_INIT_FAIL;

	m_pWindow = pWindow;

	glfwSetKeyCallback(pWindow, KeyCallBack);
	SetMouseCursorMode(mouseMode);
	glfwSetMouseButtonCallback(pWindow, MouseButtonCallBack);
	glfwSetCursorPosCallback(pWindow, MousePosCallBack);
	glfwSetScrollCallback(pWindow, MouseScrollCallBack);

	return PK_NOERROR;
}

// Remove openGL input system and all information
void CInputDevice::ClearInputSystem()
{
	unordered_map<_int, KeyInfo*>::iterator iter;
	for (iter = m_mapKeyInfos.begin(); iter != m_mapKeyInfos.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	m_mapKeyInfos.clear();

	for (iter = m_mapMouseButtonInfos.begin(); iter != m_mapMouseButtonInfos.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	m_mapMouseButtonInfos.clear();
}

// Change mouse cursor mode
void CInputDevice::SetMouseCursorMode(_int mouseMode)
{
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, mouseMode);
}

void CInputDevice::KeyCallBack(GLFWwindow* window, _int key, _int scancode, _int action, _int mods)
{
	CInputDevice::GetInstance()->SetInputKey(key, scancode, action, mods);
}

void CInputDevice::MouseButtonCallBack(GLFWwindow* window, _int button, _int action, _int mods)
{
	CInputDevice::GetInstance()->SetInputMouseButton(button, action, mods);
}

void CInputDevice::MousePosCallBack(GLFWwindow* window, _double x, _double y)
{
	CInputDevice::GetInstance()->SetMousePos(vec2(x, y));
}

void CInputDevice::MouseScrollCallBack(GLFWwindow* window, _double xOffset, _double yOffset)
{
	CInputDevice::GetInstance()->SetMouseScroll(vec2(xOffset, yOffset));
}

_bool CInputDevice::IsKeyDown(_int key)
{
	unordered_map<_int, KeyInfo*>::iterator iter = m_mapKeyInfos.find(key);

	if (iter != m_mapKeyInfos.end() && 
	 (iter->second->action == GLFW_PRESS || iter->second->action == GLFW_REPEAT))
		return true;

	return false;
}

_bool CInputDevice::IsAnyKeyDown(_int key)
{
	unordered_map<_int, KeyInfo*>::iterator iter;
	for (iter = m_mapKeyInfos.begin(); iter != m_mapKeyInfos.end(); ++iter)
	{
		if (nullptr != iter->second && iter->second->action == GLFW_RELEASE)
			return true;
	}

	return false;
}

_bool CInputDevice::IsMousePressed(_int key)
{
	unordered_map<_int, KeyInfo*>::iterator iter = m_mapMouseButtonInfos.find(key);
	if (iter != m_mapMouseButtonInfos.end())
	{
		if (nullptr != iter->second && iter->second->action == GLFW_PRESS)
			return true;
	}

	return false;
}

// Initializing the mouse position value when changing the mouse mode
void CInputDevice::InitMousePos()
{
	int iWidth, iHeight;
	glfwGetWindowSize(m_pWindow, &iWidth, &iHeight);
	vec2 newPos = vec2(iWidth / 2, iHeight / 2);
	glfwSetCursorPos(m_pWindow,  newPos.x, newPos.y);
	m_vecMousePosPrevious = m_vecMousePos = newPos;

	m_bEntered = true;
} 

// Returns the distance the mouse has moved
vec2 CInputDevice::GetMouseMovedDistance()
{
	if (m_bEntered)
	{
		m_vecMousePosPrevious = m_vecMousePos; 
		m_bEntered = false;
	}

 	vec2 movedDistance = m_vecMousePos - m_vecMousePosPrevious;
	
	if (m_bPosFixed)
	{
		int iWidth, iHeight;
		glfwGetWindowSize(m_pWindow, &iWidth, &iHeight);
		vec2 newPos = vec2(iWidth / 2, iHeight / 2);
		glfwSetCursorPos(m_pWindow, newPos.x, newPos.y);
		m_vecMousePos = newPos;
	}

	m_vecMousePosPrevious = m_vecMousePos;

	return movedDistance * m_fMouseSensitivity;
}

// Returns the distance the mouse wheel has rolled
vec2 CInputDevice::GetMouseScrollDistance()
{
	vec2 scrollDistance = m_vecScroll;
	m_vecScroll.x = 0;
	m_vecScroll.y = 0;
	return scrollDistance;
}

// Returns the world position of the mouse
vec3 CInputDevice::GetMouseWorldCoord()
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(m_pWindow, &width, &height);

	_float x = (2.f * m_vecMousePos.x) / width - 1.f;
	_float y = 1.f - (2.f * m_vecMousePos.y) / height;
	vec4 ray_clip = vec4(x, y, -1.f, 1.f);

	const mat4x4 matProj = COpenGLDevice::GetInstance()->GetProjMatrix();
	mat4x4 matProjInv = inverse(matProj);
	vec4 ray_eye = matProjInv * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

	const mat4x4 matView = COpenGLDevice::GetInstance()->GetViewMatrix();
	mat4x4 matViewInv = inverse(matView);
	vec4 ray_wor = matViewInv * ray_eye;
	vec3 ray_dir = vec3(ray_wor.x, ray_wor.y, ray_wor.z);
	ray_dir = normalize(ray_dir);

	return ray_dir;
}

// Returns the center of the world position of the mouse
vec3 CInputDevice::GetCenterMouseWorldCoord()
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(m_pWindow, &width, &height);

	_float x = width / width - 1.f;
	_float y = 1.f - height / height;
	vec4 ray_clip = vec4(x, y, -1.f, 1.f);

	const mat4x4 matProj = COpenGLDevice::GetInstance()->GetProjMatrix();
	mat4x4 matProjInv = inverse(matProj);
	vec4 ray_eye = matProjInv * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

	const mat4x4 matView = COpenGLDevice::GetInstance()->GetViewMatrix();
	mat4x4 matViewInv = inverse(matView);
	vec4 ray_wor = matViewInv * ray_eye;
	vec3 ray_dir = vec3(ray_wor.x, ray_wor.y, ray_wor.z);
	ray_dir = normalize(ray_dir);

	return ray_dir;
}

// Set custom crosshair
void CInputDevice::SetCustomCrosshair()
{
 	GLFWcursor* pCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(m_pWindow, pCursor);
}

// Remove custom crosshair
void CInputDevice::RemoveCustomCrosshair()
{
	glfwSetCursor(m_pWindow, NULL);
}

// Initialize keyboard keys
void CInputDevice::SetInputKey(_int key, _int scancode, _int action, _int mods)
{
	unordered_map<_int, KeyInfo*>::iterator iter = m_mapKeyInfos.find(key);

	if (iter == m_mapKeyInfos.end())
	{
		KeyInfo* pKey = new KeyInfo;
		pKey->scancode = scancode;
		pKey->action = action;
		pKey->preAction = action;
		pKey->mods = mods;

		m_mapKeyInfos.insert(unordered_map<_int, KeyInfo*>::value_type(key, pKey));
	}
	else
	{ 
		iter->second->scancode = scancode;
		iter->second->action = action;
		iter->second->mods = mods;
	}
}

// Initialize mouse buttons
void CInputDevice::SetInputMouseButton(_int button, _int action, _int mods)
{
	unordered_map<_int, KeyInfo*>::iterator iter = m_mapMouseButtonInfos.find(button);

	if (iter == m_mapMouseButtonInfos.end())
	{
		KeyInfo* pKey = new KeyInfo;
		pKey->scancode = 0;
		pKey->action = action;
		pKey->preAction = action;
		pKey->mods = mods;

		m_mapMouseButtonInfos.insert(unordered_map<_int, KeyInfo*>::value_type(button, pKey));
	}
	else
	{
		iter->second->action = action;
		iter->second->mods = mods;
	}
}

// Setting mouse position
void CInputDevice::SetMousePos(vec2 vPos)
{
	m_vecMousePos = vPos;
}

// Setting mouse scroll position
void CInputDevice::SetMouseScroll(vec2 vPos)
{
	m_vecScroll += vPos;
}
