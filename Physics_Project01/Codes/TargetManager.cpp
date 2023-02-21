#include "TargetManager.h"
#include "InputDevice.h"
#include "OpenGLDefines.h"
#include "CollisionMaster.h"
#include "Player.h"
#include "SceneDungeon.h"
#include "Layer.h"

SINGLETON_FUNCTION(TargetManager)
USING(Engine)
USING(std)
USING(glm)

TargetManager::TargetManager()
	: m_pTarget(nullptr), m_pScene(nullptr)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
}

TargetManager::~TargetManager()
{
}

// Call instead of destructor to manage class internal data
void TargetManager::Destroy()
{
	SafeDestroy(m_pInputDevice);
}

// Initialize
RESULT TargetManager::Ready(SceneDungeon* pScene)
{
	m_pScene = pScene;
	m_pCharacterLayer = pScene->GetLayer((_uint)LAYER_CHARACTER);

	return PK_NOERROR;
}

// Basic Update Function, check user input
void TargetManager::Update(const _float& dt)
{
	KeyCheck(dt);
}

// User Input Check
void TargetManager::KeyCheck(const _float& dt)
{
	static _bool isMouseClicked = false;
	if (m_pInputDevice->IsMousePressed(GLFW_MOUSE_BUTTON_1))
	{
		if (!isMouseClicked)
		{
			isMouseClicked = true;

			if (!FindTargetAndSet())
				MoveTarget(false);
		}
	}
	else
		isMouseClicked = false;

	static _bool isMouseRightClicked = false;
	if (m_pInputDevice->IsMousePressed(GLFW_MOUSE_BUTTON_2))
	{
		if (!isMouseRightClicked)
		{
			isMouseRightClicked = true;

			if (!FindTargetAndSet())
				MoveTarget(true);
		}
	}
	else
		isMouseRightClicked = false;

	static _bool is1Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_1))
	{
		if (!is1Down)
		{
			is1Down = true;
			ReleaseTarget();
		}
	}
	else
		is1Down = false;
}

// Check if the user's mouse click has selected a character
_bool TargetManager::FindTargetAndSet()
{
	if (nullptr == m_pCharacterLayer || nullptr == m_pScene)
		return false;

	vec3 vCameraPos = m_pScene->GetCameraPos();
	vec3 vDir = m_pInputDevice->GetMouseWorldCoord();

	vector<CGameObject*> vecPicking;
	list<CGameObject*>* listObj = m_pCharacterLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	for (iter = listObj->begin(); iter != listObj->end(); ++iter)
	{
		if ((*iter)->GetLock())
			continue;

		if (CCollisionMaster::GetInstance()->IntersectRayToBoundingBox(
			(*iter)->GetBoundingBox(),
			(*iter)->GetTransform(), vCameraPos, vDir))
		{
			vecPicking.push_back(*iter);
		}
	}

	if (vecPicking.size() > 0)
	{
		_float distanceMin = FLT_MAX; _int index = 0;
		for (int i = 0; i < vecPicking.size(); ++i)
		{
			_float dis = distance(vCameraPos, vecPicking[i]->GetPosition());
			if (dis < distanceMin)
			{
				distanceMin = dis;
				index = i;
			}
		}
		
		ReleaseTarget();
		m_pTarget = dynamic_cast<Player*>(vecPicking[index]);
		m_pTarget->SetTarget(true);
		return true;
	}

	return false;
}

// Move the target
void TargetManager::MoveTarget(_bool bSnap)
{
	if (nullptr == m_pTarget)
		return;

	vec3 vCameraPos = m_pScene->GetCameraPos();
	vec3 vDir = m_pInputDevice->GetMouseWorldCoord();
	vec3 vDest = vec3(0.f);
	if (CCollisionMaster::GetInstance()->IntersectRayToVirtualPlane(1000.f, vCameraPos, vDir, vDest))
	{
		vec3 vPos = m_pTarget->GetPosition();
		vDest.y = vPos.y;

		if (bSnap)
		{
			if (0 > vDest.x)
				vDest.x = (_int)((vDest.x - 5) / 10) * 10;
			else
				vDest.x = (_int)((vDest.x + 5) / 10) * 10;

			if (0 > vDest.z)
				vDest.z = (_int)((vDest.z - 5) / 10) * 10;
			else
				vDest.z = (_int)((vDest.z + 5) / 10) * 10;
		}

		m_pTarget->SetTargetPos(vDest);
	}

}

// Release the target
void TargetManager::ReleaseTarget()
{
	if (nullptr != m_pTarget)
	{
		m_pTarget->SetTarget(false);
		m_pTarget = nullptr;
	}
}
