#ifndef _MESH_H_
#define _MESH_H_

#include "Component.h"
#include "EngineStruct.h"
#include "glm\mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

class CVIBuffer;
class CBoundingBox;
class CTexture;
class CShader;
class CTransform;
class COpenGLDevice;
class CAnimController;

// Components with 3D mesh file information
class ENGINE_API CMesh : public CComponent
{
private:
	COpenGLDevice*				m_pOpenGLDevice;
	CVIBuffer*					m_pVIBuffer;
	CBoundingBox*				m_pBoundingBox;
	CTexture*					m_pDiffTexture;
	CTexture*					m_pNormalTexture;
	CShader*					m_pShader;
	CTransform*					m_pParentTransform;

	std::string					m_textureFileName;
	_uint						m_iTriNum;
	TRIANGLE*					m_pTriangles;

	_bool						m_bWireFrame;
	_bool						m_bSelected;
	_bool						m_bDebug;
	_bool						m_bTransparency;
	_bool						m_bBiilboard;

	CAnimController*			m_pAnimController;
	std::string					m_initSize;
	std::string					m_meshType;

private:
	explicit CMesh();
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	// Basic Render Function, render 3D mesh with shader/texture information
	virtual void Render();
private:
	// Call instead of destructor to manage class internal data
	virtual void Destroy();

public:
	CBoundingBox* GetBoundingBox()							{ return m_pBoundingBox; }
	TRIANGLE* GetTriangleArray()							{ return m_pTriangles; }
	_uint GetTriangleNumber()								{ return m_iTriNum; }
	CShader* GetShader()									{ return m_pShader; }
	std::string GetTexName()								{ return m_textureFileName; }
	std::string GetInitSize()								{ return m_initSize; }
	std::string GetMeshType()								{ return m_meshType; }
	void SetTransform(CTransform* transform)				{ m_pParentTransform = transform; }
	void SetWireFrame(_bool wire)							{ m_bWireFrame = wire; }
	void SetSelcted(_bool select)							{ m_bSelected = select; }
	void SetTransparency(_bool value)						{ m_bTransparency = value; }
	void SetDebugBox(_bool value)							{ m_bDebug = value; }
	void SetBillboard(_bool value)							{ m_bBiilboard = value; }
	void SetAnimController(CAnimController* pController)	{ m_pAnimController = pController; }
	// Set diffuse texture
	void SetTexture(std::string texID_diff);

private:
	RESULT Ready(std::string ID, std::string filePath, std::string fileName, eModelType type, std::string shaderID, std::string initSize, std::string meshType, std::string texID_Diff, std::string texID_Normal);
	RESULT Ready_VIBuffer(eModelType type, std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);
	void Ready_Texture_Diff(std::string texID);
	void Ready_Texture_Normal(std::string texID);
	void Ready_Shader(std::string shaderID);

public:
	virtual CComponent* Clone();
	static CMesh* Create(std::string ID, std::string filePath, std::string fileName, eModelType type, std::string shaderID, std::string initSize, std::string meshType, std::string texID_Diff, std::string texID_Normal);
};

NAMESPACE_END

#endif //_MESH_H_