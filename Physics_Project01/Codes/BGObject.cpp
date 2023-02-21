#include "BGObject.h"
#include "ComponentMaster.h"
#include "Layer.h"

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "OpenGLDefines.h"
#include "InputDevice.h"
#include "SoundMaster.h"
#include "Shader.h"
#include "Light.h"
#include "RigidBody.h"
#include "Function.h"


USING(Engine)
USING(glm)
USING(std)

BGObject::BGObject()
	: m_pMesh(nullptr)
{
	m_bDebug = false;
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
}

BGObject::~BGObject()
{
}

void BGObject::SetRigidBody(Engine::iRigidBody* pBody)
{
	m_pRigidBody = pBody;
	if (nullptr != m_pRigidBody)
		m_pRigidBody->SetTransform(m_pTransform);
}

void BGObject::SetTransperancy()
{
	if (nullptr != m_pMesh)
		m_pMesh->SetTransparency(true);
}

void BGObject::AddForceToRigidBody(vec3 vPos)
{
	m_pRigidBody->ApplyForce(vPos);
}

void BGObject::KeyCheck(const _float& dt)
{
	//if (nullptr == m_pInputDevice || nullptr == m_pTransform || nullptr == m_pRigidBody)
	//	return;

	//vec3 vForce(0.f);

	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_W))
	//	vForce.z += 20.f;
	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_A))
	//	vForce.x += 20.f;
	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_S))
	//	vForce.z -= 20.f;
	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_D))
	//	vForce.x -= 20.f;

	//m_pRigidBody->ApplyForce(vForce);
}

// Basic Update Function
void BGObject::Update(const _float& dt)
{
	if (m_bEnable)
	{
		//if (m_bSelected)
		//	KeyCheck(dt);

		m_pTransform->SetPosition(m_pRigidBody->GetPosition());
		m_pTransform->SetQuaternion(m_pRigidBody->GetRotation());

		//vec3 vPos = m_pTransform->GetPosition();
		//if (vPos.x < -32.f)
		//	m_pRigidBody->ApplyForce(vec3(20.f, 0.f, 0.f));
		//if (vPos.x > 32.f)
		//	m_pRigidBody->ApplyForce(vec3(-20.f, 0.f, 0.f));
		//if (vPos.z < -32.f)
		//	m_pRigidBody->ApplyForce(vec3(0.f, 0.f, 20.f));
		//if (vPos.z > 32.f)
		//	m_pRigidBody->ApplyForce(vec3(0.f, 0.f, -20.f));

		if (nullptr != m_pMesh)
			m_pMesh->SetSelcted(m_bSelected);

		CGameObject::Update(dt);

		if (nullptr != m_pRenderer)
			m_pRenderer->AddRenderObj(this);
	}
}

// Basic Render Function
void BGObject::Render()
{
	CGameObject::Render();
}

// Call instead of destructor to manage class internal data
void BGObject::Destroy()
{
	SafeDestroy(m_pInputDevice);

	CGameObject::Destroy();
}

// Initialize
RESULT BGObject::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;

	//Clone.Mesh
 	m_pMesh = CloneComponent<CMesh*>(meshID);
	if (nullptr != m_pMesh)
	{
		AttachComponent("Mesh", m_pMesh);
		m_pMesh->SetTransform(m_pTransform);
		m_pBoundingBox = m_pMesh->GetBoundingBox();
		if (nullptr != m_pBoundingBox)
			m_pBoundingBox->SetTransform(m_pTransform);
		m_pMesh->SetWireFrame(false);
		m_pMesh->SetDebugBox(false);
	}

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);

		quat qRot = quat(vec3(radians(vRot.x), radians(vRot.y), radians(vRot.z)));
		m_pTransform->SetQuaternion(qRot);
		m_pTransform->Update(0);
	}

	return PK_NOERROR;
}

// Create an instance
BGObject* BGObject::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	BGObject* pInstance = new BGObject();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, vPos, vRot, vScale))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
