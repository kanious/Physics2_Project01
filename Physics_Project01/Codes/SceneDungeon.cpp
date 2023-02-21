#include "SceneDungeon.h"
#include "Function.h"
#include "glm\vec3.hpp"
#include "InputDevice.h"
#include "DefaultCamera.h"
#include "JsonParser.h"
#include "Layer.h"
#include "ComponentMaster.h"
#include "Component.h"
#include "Shader.h"
#include "LightMaster.h"
#include "BGObject.h"
#include "Define.h"
#include "UIManager.h"
#include "Enums.h"
#include "SkyBox.h"
#include "Renderer.h"
#include "ObjectFactory.h"
#include "SoundMaster.h"

#include "PhysicsDefines.h"

#include <functional>
#include <sstream>
#include <atlconv.h>
 

USING(Engine)
USING(glm)
USING(std)

SceneDungeon::SceneDungeon()
	: m_pSkyBox(nullptr)
	, m_pDefaultCamera(nullptr), m_vCameraSavedPos(vec3(0.f)), m_vCameraSavedRot(vec3(0.f)), m_vCameraSavedTarget(vec3(0.f))
	, m_pCharacterLayer(nullptr), m_pPFactory(nullptr), m_pPWorld(nullptr), m_iTargetIndex(0)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
	m_pUIManager = UIManager::GetInstance(); m_pUIManager->AddRefCnt();
	m_vecTargets.clear();

	m_ObjListFileName = "physicsMapObjects.json";
	m_LightListFileName = "lights.xml";
}

SceneDungeon::~SceneDungeon()
{
}

// Call instead of destructor to manage class internal data
void SceneDungeon::Destroy()
{
	SafeDestroy(m_pInputDevice);
	SafeDestroy(m_pUIManager);
	SafeDestroy(m_pPFactory);
	SafeDestroy(m_pPWorld);

	SafeDestroy(m_pSkyBox);

	m_vecTargets.clear();

	CScene::Destroy();
}

// Basic Update Function
void SceneDungeon::Update(const _float& dt)
{
	if (nullptr != m_pSkyBox)
		CRenderer::GetInstance()->AddRenderObj(m_pSkyBox);

	if (nullptr != m_pPWorld)
		m_pPWorld->Update(dt);

	KeyCheck();

	CLightMaster::GetInstance()->SetLightInfo();

	CScene::Update(dt);
}

// Basic Render Function
void SceneDungeon::Render()
{
	if (nullptr != m_pUIManager)
		m_pUIManager->RenderUI();
}

// Return current camera position
vec3 SceneDungeon::GetCameraPos()
{
	if (nullptr != m_pDefaultCamera)
		return m_pDefaultCamera->GetCameraEye();

	return vec3(0.f);
}

void SceneDungeon::CollisionCallback()
{
	CSoundMaster::GetInstance()->PlaySound("Ball");
}

string SceneDungeon::GetCurrentTargetName()
{
	if (0 == m_iTargetIndex)
		return "NULL";

	BGObject* target = m_vecTargets[m_iTargetIndex];
	return target->GetMeshName();
}

// Check User input
void SceneDungeon::KeyCheck()
{
	static _bool isF3Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F3))
	{
		if (!isF3Down)
		{
			isF3Down = true;

			if (nullptr != m_pPWorld)
				m_pPWorld->ApplyRandomForce();
		}
	}
	else
		isF3Down = false;

	static _bool isF2Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F2))
	{
		if (!isF2Down)
		{
			isF2Down = true;

			if (nullptr != m_pPWorld)
				m_pPWorld->ResetAllRigidBodies();
		}
	}
	else
		isF2Down = false;

	static _bool isF1Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F1))
	{
		if (!isF1Down)
		{
			isF1Down = true;

			m_iTargetIndex = 0;
			m_pDefaultCamera->SetTargetObject(m_vecTargets[m_iTargetIndex]);
		}
	}
	else
		isF1Down = false;

	static _bool isTabDown = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_TAB))
	{
		if (!isTabDown)
		{
			isTabDown = true;
			
			++m_iTargetIndex;
			if (m_iTargetIndex >= m_vecTargets.size())
				m_iTargetIndex = 0;
			m_pDefaultCamera->SetTargetObject(m_vecTargets[m_iTargetIndex]);
		}
	}
	else
		isTabDown = false;
}

// Saves camera position
void SceneDungeon::SetDefaultCameraSavedPosition(vec3 vPos, vec3 vRot, vec3 target)
{
	m_vCameraSavedPos.x = vPos.x;
	m_vCameraSavedPos.y = vPos.y;
	m_vCameraSavedPos.z = vPos.z;

	m_vCameraSavedRot.x = vRot.x;
	m_vCameraSavedRot.y = vRot.y;
	m_vCameraSavedRot.z = vRot.z;

	m_vCameraSavedTarget.x = target.x;
	m_vCameraSavedTarget.y = target.y;
	m_vCameraSavedTarget.z = target.z;
}

// Reset camera position
void SceneDungeon::ResetDefaultCameraPos()
{
	if (nullptr != m_pDefaultCamera)
	{
		m_pDefaultCamera->SetCameraEye(m_vCameraSavedPos);
		m_pDefaultCamera->SetCameraRot(m_vCameraSavedRot);
		m_pDefaultCamera->SetCameraTarget(m_vCameraSavedTarget);
	}
}

// Initialize
RESULT SceneDungeon::Ready(string dataPath)
{
	m_DataPath = dataPath;

	// Physics
	m_pPFactory = CPhysicsFactory::Create();
	m_pPWorld = m_pPFactory->CreateWorld(bind(&SceneDungeon::CollisionCallback, this));
	if (nullptr != m_pPWorld)
		m_pPWorld->SetGravity(vec3(0.f, -9.81f, 0.f));

	// Sound
	CSoundMaster::GetInstance()->SetVolume("Ball", 2.f);

	// GameObjects
	RESULT result = PK_NOERROR;
	result = ReadyLayerAndGameObject();
	if (PK_NOERROR != result)
		return result;

	// Light
	CComponent* shader = CComponentMaster::GetInstance()->FindComponent("DefaultShader");
	_uint shaderID = 0;
	if (nullptr != shader)
		shaderID = dynamic_cast<CShader*>(shader)->GetShaderProgram();
	CLightMaster::GetInstance()->SetShader(shaderID);
	CLightMaster::GetInstance()->LoadLights(m_DataPath, m_LightListFileName);

	// Set Camera info to Shader
	if (nullptr != m_pDefaultCamera)
		m_pDefaultCamera->AddShaderLocation("DefaultShader");

	// UI
	if (nullptr != m_pUIManager)
		m_pUIManager->Ready(this);

	if (nullptr == m_pSkyBox)
	{
		stringstream ss, ss2;
		ss << m_DataPath << "Texture\\SkyBox\\";

		vector<string> faces;
		ss2.str(""); ss2 << ss.str() << "right.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "left.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "top.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "bottom.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "front.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "back.jpg"; faces.push_back(ss2.str());

		CComponent* skyboxShader = CComponentMaster::GetInstance()->FindComponent("SkyBoxShader");
		m_pSkyBox = CSkyBox::Create(faces, dynamic_cast<CShader*>(skyboxShader));
	}

	return PK_NOERROR;
}

// Initialize GameObjects
RESULT SceneDungeon::ReadyLayerAndGameObject()
{
	//Create.Camera
	CLayer* pLayer = GetLayer((_uint)LAYER_INTERACTIVE_OBJECT);
	if (nullptr != pLayer)
	{
		vec3 vPos = vec3(0.f, 0.f, 0.f);
		vec3 vRot = vec3(0.f, 0.f, 0.f);
		vec3 vScale = vec3(1.f);

		m_pDefaultCamera = ObjectFactory::CreateCamera(
			(_uint)SCENE_3D, pLayer->GetTag(),
			(_uint)OBJ_CAMERA, pLayer,
			vPos, vRot, vScale, 0.6f, 0.1f, 1000.f);
	}

	//Create.BackgroundLayer 
	LoadObjects();

	return PK_NOERROR;
}

// Load Objects from json file
void SceneDungeon::LoadObjects()
{
	CLayer* bgLayer = GetLayer((_uint)LAYER_BACKGROUND_OBJECT);
	m_pCharacterLayer = GetLayer((_uint)LAYER_CHARACTER);

	if (nullptr == bgLayer)
		return;

	BGObject* newObject = nullptr;
	m_vecTargets.push_back(nullptr);

	bgLayer->RemoveAllGameObject();
	vector<CJsonParser::sObjectData> vecObjects;
	CJsonParser::sObjectData cameraData;
	CJsonParser::GetInstance()->LoadObjectList(m_DataPath, m_ObjListFileName, vecObjects, cameraData);
	vector<CJsonParser::sObjectData>::iterator iter;
	for (iter = vecObjects.begin(); iter != vecObjects.end(); ++iter)
	{
		if (!strcmp("static_obj", iter->LAYERTYPE.c_str()))
		{
			newObject = ObjectFactory::CreateBGObject(
				(_uint)SCENE_3D,
				bgLayer->GetTag(),
				(_uint)OBJ_BACKGROUND,
				bgLayer,
				iter->ID, iter->POSITION, iter->ROTATION, iter->SCALE);

			newObject->SetTransperancy();

			CRigidBodyDesc newDesc;
			newDesc.isStatic = true;
			newDesc.isGround = iter->ISGROUND;
			newDesc.mass = 0.f;
			newDesc.position = iter->POSITION;
			quat qRot = quat(vec3(radians(iter->ROTATION.x), radians(iter->ROTATION.y), radians(iter->ROTATION.z)));
			newDesc.rotation = qRot;
			newDesc.linearVelocity = vec3(0.f);

			iShape* shape = CPlaneShape::Create(eShapeType::Plane, iter->NORMAL, 0.f);

			iRigidBody* rigidBody = m_pPFactory->CreateRigidBody(newDesc, shape);
			newObject->SetRigidBody(rigidBody);
			m_pPWorld->AddBody(rigidBody);
		}
		else if (!strcmp("interative_obj", iter->LAYERTYPE.c_str()))
		{
			newObject = ObjectFactory::CreateBGObject(
				(_uint)SCENE_3D,
				bgLayer->GetTag(),
				(_uint)OBJ_BACKGROUND,
				bgLayer,
				iter->ID, iter->POSITION, iter->ROTATION, iter->SCALE);

			CRigidBodyDesc newDesc;
			newDesc.isStatic = false;
			newDesc.mass = iter->SCALE.x;
			newDesc.position = iter->POSITION;
			newDesc.linearVelocity = vec3(0.f);

			iShape* shape = CSphereShape::Create(eShapeType::Sphere, iter->SCALE.x);

			iRigidBody* rigidBody = m_pPFactory->CreateRigidBody(newDesc, shape);
			newObject->SetRigidBody(rigidBody);
			m_pPWorld->AddBody(rigidBody);

			m_vecTargets.push_back(newObject);
		}
	}
	vecObjects.clear();

	m_iTargetIndex = m_vecTargets.size() - 1;
	m_pDefaultCamera->SetTargetObject(m_vecTargets[m_iTargetIndex]);


	if (nullptr != newObject)
	{
		newObject->SetSelected(true);
		m_pDefaultCamera->SetTargetObject(newObject);
	}

	if (nullptr != m_pDefaultCamera)
	{
		SetDefaultCameraSavedPosition(cameraData.POSITION, cameraData.ROTATION, cameraData.SCALE);
		m_pDefaultCamera->SetCameraEye(cameraData.POSITION);
		m_pDefaultCamera->SetCameraRot(cameraData.ROTATION);
		m_pDefaultCamera->SetCameraTarget(cameraData.SCALE);
	}
}

// Create an instance
SceneDungeon* SceneDungeon::Create(string dataPath)
{
	SceneDungeon* pInstance = new SceneDungeon();
	if (PK_NOERROR != pInstance->Ready(dataPath))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
