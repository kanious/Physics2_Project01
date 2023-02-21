#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Component.h"
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

// Component for camera function
class ENGINE_API CCamera : public CComponent
{
protected:
	glm::vec3			m_vEye;
	glm::vec3			m_vTarget;
	glm::vec3			m_vUp;
	glm::mat4x4			m_matView;

	_float				m_fFov;
	_float				m_fRatio;
	_float				m_fNear;
	_float				m_fFar;
	glm::mat4x4			m_matProj;


protected:
	explicit CCamera();
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();
public:
	virtual void Update(const _float& dt);
private:
	virtual void Destroy();

public:
	const glm::vec3 GetCameraEye() { return m_vEye; }
	const glm::vec3 GetCameraTarget() { return m_vTarget; }
	const glm::vec3 GetCameraUp() { return m_vUp; }
	const glm::vec3 GetUpVector() { return m_vUp; }
	const glm::mat4x4 GetViewMatrix() { return m_matView; }
	const _float GetFieldOfView() { return m_fFov; }
	const _float GetRatio() { return m_fRatio; }
	const _float GetNear() { return m_fNear; }
	const _float GetFar() { return m_fFar; }
	const glm::mat4x4 GetProjMatrix() { return m_matProj; }
	void SetCameraEye(glm::vec3 vEye);
	void SetCameraTarget(glm::vec3 vTarget);
	void SetCameraUp(glm::vec3 vUp);
	void SetFieldOfView(_float fov);
	void SetRatio();
	void SetNear(_float fNear);
	void SetFar(_float fFar);
private:
	void CalculateViewMatrix();
	void CalculateProjMatrix();

private:
	RESULT Ready();
public:
	virtual CComponent* Clone();
	static CCamera* Create();
	void InitCameraSetting(glm::vec3 vEye, glm::vec3 vTarget, glm::vec3 vUp, _float fov, _float fNear, _float fFar);
};

NAMESPACE_END

#endif //_CAMERA_H_