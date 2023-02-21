#ifndef _VIBUFFER_H_
#define _VIBUFFER_H_

#include "Base.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

// Class for 3D vertex/index data
class ENGINE_API CVIBuffer : public CBase
{
private:
	std::string			m_strName;
	_uint				m_iVAO_ID;
private:
	_uint				m_iVB_ID;
	_uint				m_iNumVtx;
	_uint				m_iIB_ID;
	_uint				m_iNumIdx;
private:
	_bool				m_bWireFrame;


public:
	explicit CVIBuffer();
	virtual ~CVIBuffer();

public:
	virtual void Render();
	virtual void Destroy();

public:
	void SetWireFrame(_bool wireFrame) { m_bWireFrame = wireFrame; }

private:
	RESULT Ready(_uint numVTX, VTX* pVertices, _uint numIDX, _uint* pIndices, eModelType type);
	void Ready_Vertex_To_Shader();
	void Ready_xyz();
	void Ready_xyz_normal();
	void Ready_xyz_normal_texUV();

public:
	static CVIBuffer* Create(_uint numVTX, VTX* pVertices, _uint numIDX, _uint* pIndices, eModelType type);
};

NAMESPACE_END

#endif //_VIBUFFER_H_