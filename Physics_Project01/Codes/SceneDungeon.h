#ifndef _SCENEDUNGEON_H_
#define _SCENEDUNGEON_H_

#include "Scene.h"
#include "glm\vec3.hpp"
#include "CollisionHandler.h"

namespace Engine
{
	class CInputDevice;
	class CLayer;
	class CSkyBox;
	class iPhysicsFactory;
	class iPhysicsWorld;
	class CRigidBody;
}
class UIManager;
class DefaultCamera;
class BGObject;

// A game scene class that inherits from the engine's CScene class
class SceneDungeon : public Engine::CScene
{
private:
	Engine::CInputDevice*		m_pInputDevice;
	Engine::CLayer*				m_pCharacterLayer;
	Engine::CSkyBox*			m_pSkyBox;
	UIManager*					m_pUIManager;
	DefaultCamera*				m_pDefaultCamera;

	glm::vec3					m_vCameraSavedPos;
	glm::vec3					m_vCameraSavedRot;
	glm::vec3					m_vCameraSavedTarget;

	Engine::iPhysicsFactory*	m_pPFactory;
	Engine::iPhysicsWorld*		m_pPWorld;

	std::vector<BGObject*>		m_vecTargets;
	_uint						m_iTargetIndex;


private:
	explicit SceneDungeon();
	virtual ~SceneDungeon();
	virtual void Destroy();
public:
	virtual void Update(const _float& dt);
	virtual void Render();
	
public:
	glm::vec3 GetCameraPos();
	void CollisionCallback();
	std::string GetCurrentTargetName();
private:
	void KeyCheck();
	void SetDefaultCameraSavedPosition(glm::vec3 vPos, glm::vec3 vRot, glm::vec3 target);
	void ResetDefaultCameraPos();

private:
	RESULT Ready(std::string dataPath);
	RESULT ReadyLayerAndGameObject();
	void LoadObjects();
public:
	static SceneDungeon* Create(std::string dataPath);

};

#endif //_SCENEDUNGEON_H_