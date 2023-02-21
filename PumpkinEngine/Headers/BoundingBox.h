#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "Component.h"
#include "EngineStruct.h"
#include "glm\mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

class CVIBuffer;
class COpenGLDevice;
class CShader;

class CTransform;

// Component for collision checking
class ENGINE_API CBoundingBox : public CComponent
{
public:
	CVIBuffer*			m_pVIBuffer;
	VTX*				m_pVertices;
	glm::vec3			m_vCenter;
	glm::vec3			m_vHalfExtents;
	glm::vec3			m_vMin;
	glm::vec3			m_vMax;
	glm::vec3			m_vOriginCenter;
	glm::vec3			m_vOriginHalfExtents;
	glm::vec3			m_vOriginMin;
	glm::vec3			m_vOriginMax;
	glm::vec3			m_vMinWorld;
	glm::vec3			m_vMaxWorld;
	std::string			m_shaderID;

	COpenGLDevice*		m_pOpenGLDevice;
	CShader*			m_pShader;
	CTransform*			m_pParentTransform;

private:
	explicit CBoundingBox();
	explicit CBoundingBox(const CBoundingBox& rhs);
	virtual ~CBoundingBox();
public:
	virtual void Render();
	void RenderWithoutParent();
	void RenderWithParent(CTransform* parent);
private:
	virtual void Destroy();

public:
	VTX* GetVertices()							{ return m_pVertices; }
	void SetTransform(CTransform* transform)	{ m_pParentTransform = transform; }
	void UpdateBoundingBox(glm::mat4x4& parentWorldMatrix);
	void SetColor(glm::vec3 vColor);

private:
	RESULT Ready(glm::vec3 min, glm::vec3 max, std::string shaderID);
public:
	virtual CComponent* Clone();
	static CBoundingBox* Create(glm::vec3 min, glm::vec3 max, std::string shaderID);
};

NAMESPACE_END

#endif //_COLLISIONMASTER_H_