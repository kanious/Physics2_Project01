#include "DefaultCamera.h"
#include "ComponentMaster.h"
#include "Layer.h"

#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "OpenGLDefines.h"
#include "InputDevice.h"
#include "Shader.h"
#include "ComponentMaster.h"
#include "Component.h"
#include "Function.h"

#include "BGObject.h"

USING(Engine)
USING(glm)
USING(std)

DefaultCamera::DefaultCamera()
	: m_pCamera(nullptr), m_bMouseEnable(true), m_fAngleY(0.f), m_fAngleX(0.f)
	, m_fSpeed(30.f), m_fRotSpeed(20.f), m_pTarget(nullptr), m_fDistance(30.f)
	, m_matWorld(mat4x4(1.f)), m_fMaxDistance(100.f), m_fMinDistance(8.f)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
}

DefaultCamera::~DefaultCamera()
{
}

vec3 DefaultCamera::GetCameraEye()
{
	if (nullptr == m_pCamera)
		return vec3(0.f);

	return m_pCamera->GetCameraEye();
}

vec3 DefaultCamera::GetCameraRot()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetRotation();
}

vec3 DefaultCamera::GetCameraTarget()
{
	if (nullptr == m_pCamera)
		return vec3(0.f);

	return m_pCamera->GetCameraTarget();
}

void DefaultCamera::SetMouseEnable(_bool enable)
{
	m_bMouseEnable = enable;
	if (enable)
		m_pInputDevice->SetMouseCursorMode(GLFW_CURSOR_NORMAL);
	else
		m_pInputDevice->SetMouseCursorMode(GLFW_CURSOR_DISABLED);

	m_pInputDevice->InitMousePos();
}

void DefaultCamera::SetCameraEye(glm::vec3 eye)
{
	if (nullptr == m_pCamera || nullptr == m_pTransform || nullptr == m_pInputDevice)
		return;

	m_pTransform->SetPosition(eye);
	m_pCamera->SetCameraEye(eye);
}

void DefaultCamera::SetCameraRot(glm::vec3 rot)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotation(rot);
}

void DefaultCamera::SetCameraTarget(glm::vec3 target)
{
	if (nullptr == m_pCamera || nullptr == m_pTransform || nullptr == m_pInputDevice)
		return;

	vec3 vEye = m_pCamera->GetCameraEye();
	vec3 vDir = target - vEye;
	vDir = normalize(vDir);
	m_pCamera->SetCameraTarget(vEye + vDir);
	m_pInputDevice->InitMousePos();

	m_fAngleX = m_pTransform->GetRotationX();
	m_fAngleY = m_pTransform->GetRotationY();
}

void DefaultCamera::SetTargetObject(BGObject* obj)
{
	if (nullptr != m_pTarget)
		m_pTarget->SetSelected(false);

	m_pTarget = obj;

	if (nullptr != m_pTarget)
		m_pTarget->SetSelected(true);
}

// Save shaders to deliver camera information
void DefaultCamera::AddShaderLocation(string shaderTag)
{
	CComponent* shader = CComponentMaster::GetInstance()->FindComponent(shaderTag);
	if (nullptr != shader)
	{
		_uint shaderID = dynamic_cast<CShader*>(shader)->GetShaderProgram();
		_uint shaderLocation = glGetUniformLocation(shaderID, "eyeLocation");
		m_vecShaders.push_back(shaderLocation);
	}
}

// Check user input
void DefaultCamera::KeyCheck(const _float& dt)
{
	if (nullptr == m_pInputDevice || nullptr == m_pTransform)
		return;

	vec2 vMousePos = m_pInputDevice->GetMouseMovedDistance();
	vec2 vScrollPos = m_pInputDevice->GetMouseScrollDistance();

	m_fAngleY -= vMousePos.x * 2.f;
	if (m_fAngleY > 360.f)
		m_fAngleY -= 360.f;
	if (m_fAngleY < 0)
		m_fAngleY += 360.f;

	m_fDistance -= vScrollPos.y * 5.f;
	if (m_fDistance < m_fMinDistance)
		m_fDistance = m_fMinDistance;
	if (m_fDistance > m_fMaxDistance)
		m_fDistance = m_fMaxDistance;

	if (nullptr == m_pTarget)
		return;

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_W))
	{
		vec3 vDir = m_matWorld[2];
		m_pTarget->AddForceToRigidBody(vDir * m_fSpeed);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_S))
	{
		vec3 vDir = m_matWorld[2];
		m_pTarget->AddForceToRigidBody(vDir * -m_fSpeed);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_A))
	{
		vec3 vRight = m_matWorld[0];
		m_pTarget->AddForceToRigidBody(vRight * m_fSpeed);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_D))
	{
		vec3 vRight = m_matWorld[0];
		m_pTarget->AddForceToRigidBody(vRight * -m_fSpeed);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_SPACE))
	{
		vec3 vUp = m_matWorld[1];
		m_pTarget->AddForceToRigidBody(vUp * 50.f);
	}
}

void DefaultCamera::FollowCenter()
{
	vec3 vTargetPos = vec3(0.f);
	m_pCamera->SetCameraTarget(vTargetPos);

	vec3 vLook(0.f, 0.f, 1.f);
	m_matWorld = mat4x4(1.f);
	m_matWorld = rotate(m_matWorld, radians(m_fAngleY), vec3(0.f, 1.f, 0.f));
	vLook = mat3x3(m_matWorld) * vLook;

	vec3 vEyePos = vTargetPos;
	vEyePos += -vLook * m_fDistance;
	vEyePos.y = m_fDistance;

	m_pCamera->SetCameraEye(vEyePos);
}

void DefaultCamera::FollowTarget()
{
	vec3 vTargetPos = m_pTarget->GetPosition();
	m_pCamera->SetCameraTarget(vTargetPos);

	vec3 vLook(0.f, 0.f, 1.f);
	m_matWorld = mat4x4(1.f);
	m_matWorld = rotate(m_matWorld, radians(m_fAngleY), vec3(0.f, 1.f, 0.f));
	vLook = mat3x3(m_matWorld) * vLook;

	vec3 vEyePos = vTargetPos;
	vEyePos += -vLook * m_fDistance;
	vEyePos.y = m_fDistance;

	m_pCamera->SetCameraEye(vEyePos);
}

// Basic Update Function
void DefaultCamera::Update(const _float& dt)
{
	KeyCheck(dt);

	if (nullptr == m_pTarget)
		FollowCenter();
	else
		FollowTarget();

	CGameObject::Update(dt);

	vec3 vEye = m_pCamera->GetCameraEye();

	for (int i = 0; i < m_vecShaders.size(); ++i)
		glUniform4f(m_vecShaders[i], vEye.x, vEye.y, vEye.z, 1.0f);
}

// Call instead of destructor to manage class internal data
void DefaultCamera::Destroy()
{
	SafeDestroy(m_pInputDevice);

	CGameObject::Destroy();
}

// Initialize
RESULT DefaultCamera::Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, vec3 vPos, vec3 vRot, vec3 vScale, _float fov, _float fNear, _float fFar)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;

	SetMouseEnable(false);

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;

	//Clone.Camera
	m_pCamera = CloneComponent<CCamera*>("Camera");
	AttachComponent("Camera", m_pCamera);

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosition(vPos);
		m_pTransform->SetRotation(vRot);
		m_pTransform->SetScale(vScale);
		m_pTransform->Update(0);

		if (nullptr != m_pCamera)
			m_pCamera->InitCameraSetting(m_pTransform->GetPosition(), m_pTransform->GetLookVector(), vec3(0.f, 1.f, 0.f), fov, fNear, fFar);
	}
	return PK_NOERROR;
}

// Create an instance
DefaultCamera* DefaultCamera::Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, vec3 vPos, vec3 vRot, vec3 vScale, _float fov, _float fNear, _float fFar)
{
	DefaultCamera* pInstance = new DefaultCamera();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, vPos, vRot, vScale, fov, fNear, fFar))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}