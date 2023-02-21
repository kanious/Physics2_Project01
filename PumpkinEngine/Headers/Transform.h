#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "OpenGLDefines.h"

NAMESPACE_BEGIN(Engine)

// Component for position/rotation/scale data
class ENGINE_API CTransform : public CComponent
{
private:
	glm::mat4x4				m_matWorld;
	glm::vec3				m_vPos;
	glm::vec3				m_vRot;
	glm::vec3				m_vScale;
	glm::vec3				m_vRevolve;
	glm::quat				m_qRot;
	CTransform*				m_pParentTransform;

private:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();
public:
	// Basic Update Function, calculate world matrix
	virtual void Update(const _float& dt);
private:
	// Call instead of destructor to manage class internal data
	virtual void Destroy();

public:
	// Get world matrix
	const glm::mat4x4* GetWorldMatrix()				{ return &m_matWorld; }
	// Get position information
	const glm::vec3 GetPosition()					{ return m_vPos; }
	// Final position information calculated with all parent information
	const glm::vec3 GetPositionWithParent();
	const _float GetPositionX()						{ return m_vPos.x; }
	const _float GetPositionY()						{ return m_vPos.y; }
	const _float GetPositionZ()						{ return m_vPos.z; }
	// Get rotation information
	const glm::vec3 GetRotation()					{ return m_vRot; }
	// Final rotation information calculated with all parent information
	const glm::vec3 GetRotationWithParent();
	const _float GetRotationX()						{ return m_vRot.x; }
	const _float GetRotationY()						{ return m_vRot.y; }
	const _float GetRotationZ()						{ return m_vRot.z; }
	// Get scale information
	const glm::vec3 GetScale()						{ return m_vScale; }
	// Get revolve information
	const glm::vec3 GetRevolve()					{ return m_vRevolve; }
	// Get parent transform
	const CTransform* GetParent()					{ return m_pParentTransform; }
	// Get parent world matrix information
	const glm::mat4x4 GetParentMatrix();
	const glm::vec3 GetRightVector()				{ return normalize(m_matWorld[0]); }
	const glm::vec3 GetUpVector()					{ return normalize(m_matWorld[1]); }
	const glm::vec3 GetLookVector()					{ return normalize(m_matWorld[2]); }
	const glm::vec3 GetPositionInWorldMatrix()		{ return m_matWorld[3]; }
	// Set world matrix directly
	void SetWorldMatrix(glm::mat4x4 matWorld);
	// Set position/rotation/scale information
	void SetPosRotScale(glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
	// Set position from vector
	void SetPosition(glm::vec3 vPos);
	// Set position X
	void SetPositionX(_float x);
	// Set position Y
	void SetPositionY(_float y);
	// Set position Z
	void SetPositionZ(_float z);
	// Set rotation
	void SetRotation(glm::vec3 vRot);
	// Set rotation X
	void SetRotationX(_float x);
	// Set rotation Y
	void SetRotationY(_float y);
	// Set rotation Z
	void SetRotationZ(_float z);
	// Set scale
	void SetScale(glm::vec3 vScale);
	// Set scale X
	void SetScaleX(_float x);
	// Set scale Y
	void SetScaleY(_float y);
	// Set scale Z
	void SetScaleZ(_float z);
	// Set revolve information
	void SetRevolve(glm::vec3 vRevolve);
	// Set parent transform
	void SetParent(CTransform* pParent);
	// Set Quaternion
	void SetQuaternion(glm::quat quaternion);
	// Add position
	void AddPosition(glm::vec3 vPos);
	// Add position X
	void AddPositionX(_float x);
	// Add position Y
	void AddPositionY(_float y);
	// Add position Z
	void AddPositionZ(_float z);
	// Add rotation
	void AddRotation(glm::vec3 vRot);
	// Add rotation X
	void AddRotationX(_float x);
	// Add rotation Y
	void AddRotationY(_float y);
	// Add rotation Z
	void AddRotationZ(_float z);
	// Add scale
	void AddScale(glm::vec3 vScale);
	// Add revolve
	void AddRevolve(glm::vec3 vRevolve);
public:
	// calculate world matrix with my data
	void ComputeWorldMatrix(glm::mat4x4& matWorld, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);

private:
	// Initialize transform
	RESULT Ready();
public:
	// Clone component
	virtual CComponent* Clone();
	// Create an instance
	static CTransform* Create();
};

NAMESPACE_END

#endif //_TRANSFORM_H_