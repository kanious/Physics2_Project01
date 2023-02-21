#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "Base.h"
#include "OpenGLDefines.h"

NAMESPACE_BEGIN(Engine)

// Class to manage all user input
class ENGINE_API CInputDevice : public CBase
{
	SINGLETON(CInputDevice)

private:
	class KeyInfo
	{
	public:
		_int scancode;
		_int action;
		_int preAction;
		_int mods;
	};

private:
	GLFWwindow*								m_pWindow;
	std::unordered_map<_int, KeyInfo*>		m_mapKeyInfos;
	std::unordered_map<_int, KeyInfo*>		m_mapMouseButtonInfos;
	glm::vec2								m_vecMousePos;
	glm::vec2								m_vecMousePosPrevious;
	glm::vec2								m_vecScroll;
	_float									m_fMouseSensitivity;
	_bool									m_bEntered;
	_bool									m_bPosFixed;

private:
	explicit CInputDevice();
	virtual ~CInputDevice();
	// Call instead of destructor to manage class internal data
	void Destroy();

public:
	// Setup openGL input system
	RESULT SetupInputSystem(GLFWwindow* pWindow, _int mouseMode);
	// Remove openGL input system and all information
	void ClearInputSystem();
	// Change mouse cursor mode
	void SetMouseCursorMode(_int mouseMode);
	static void KeyCallBack(GLFWwindow* window, _int key, _int scancode, _int action, _int mods);
	static void MouseButtonCallBack(GLFWwindow* window, _int button, _int action, _int mods);
	static void MousePosCallBack(GLFWwindow* window, _double x, _double y);
	static void MouseScrollCallBack(GLFWwindow* window, _double xOffset, _double yOffset);
	_bool IsKeyDown(_int key);
	_bool IsAnyKeyDown(_int key);
	_bool IsMousePressed(_int key);
	// Initializing the mouse position value when changing the mouse mode
	void InitMousePos();
	// Returns the postion of the mouse
	glm::vec2 GetMousePos() { return m_vecMousePos; }
	// Returns the distance the mouse has moved
	glm::vec2 GetMouseMovedDistance();
	// Returns the distance the mouse wheel has rolled
	glm::vec2 GetMouseScrollDistance();
	// Returns the world position of the mouse
	glm::vec3 GetMouseWorldCoord();
	// Returns the center of the world position of the mouse
	glm::vec3 GetCenterMouseWorldCoord();
	// Set mouse sensitivity
	void SetMouseSensitivity(_float value) { m_fMouseSensitivity = value; }
	// Set custom crosshair
	void SetCustomCrosshair();
	// Remove custom crosshair
	void RemoveCustomCrosshair();
	void SetMousePosFixed(_bool value) { m_bPosFixed = value; }

private:
	// Initialize keyboard keys
	void SetInputKey(_int key, _int scancode, _int action, _int mods);
	// Initialize mouse buttons
	void SetInputMouseButton(_int button, _int action, _int mods);
	// Setting mouse position
	void SetMousePos(glm::vec2 vPos);
	// Setting mouse scroll position
	void SetMouseScroll(glm::vec2 vPos);
};

NAMESPACE_END

#endif //_INPUTMANAGER_H_