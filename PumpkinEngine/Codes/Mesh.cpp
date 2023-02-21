#include "pch.h"
#include "../Headers/Mesh.h"
#include "../Headers/VIBuffer.h"
#include "../Headers/Component.h"
#include "../Headers/ComponentMaster.h"
#include "../Headers/OpenGLDevice.h"
#include "../Headers/Texture.h"
#include "../Headers/Shader.h"
#include "../Headers/Transform.h"
#include "../Headers/BoundingBox.h"
#include "../Headers/AnimController.h"
#include <fstream>
#include <sstream>
#include <cstdlib>


USING(Engine)
USING(std)
USING(glm)

static int GetRandNum(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

CMesh::CMesh()
    : m_pOpenGLDevice(COpenGLDevice::GetInstance())
    , m_pVIBuffer(nullptr)
    , m_pBoundingBox(nullptr)
    , m_pDiffTexture(nullptr)
    , m_pNormalTexture(nullptr)
    , m_pShader(nullptr)
    , m_pParentTransform(nullptr)
    , m_bWireFrame(false)
    , m_bSelected(false)
    , m_bDebug(false)
    , m_bTransparency(false)
    , m_bBiilboard(false)
    , m_textureFileName("")
    , m_iTriNum(0)
    , m_pTriangles(nullptr)
    , m_pAnimController(nullptr)
    , m_initSize("")
    , m_meshType("")
{
    m_pOpenGLDevice->AddRefCnt();
}

CMesh::CMesh(const CMesh& rhs)
    : m_pOpenGLDevice(rhs.m_pOpenGLDevice)
    , m_pVIBuffer(rhs.m_pVIBuffer)
    , m_pDiffTexture(rhs.m_pDiffTexture)
    , m_pNormalTexture(rhs.m_pNormalTexture)
    , m_pShader(rhs.m_pShader)
    , m_pParentTransform(nullptr)
    , m_bWireFrame(rhs.m_bWireFrame)
    , m_bSelected(rhs.m_bSelected)
    , m_bDebug(rhs.m_bDebug)
    , m_bBiilboard(rhs.m_bBiilboard)
    , m_bTransparency(rhs.m_bTransparency)
    , m_textureFileName(rhs.m_textureFileName)
    , m_iTriNum(rhs.m_iTriNum)
    , m_pAnimController(nullptr)
    , m_initSize(rhs.m_initSize)
    , m_meshType(rhs.m_meshType)
{
    m_tag = rhs.m_tag;
    m_pOpenGLDevice->AddRefCnt();
    if (nullptr != m_pVIBuffer) m_pVIBuffer->AddRefCnt();
    if (nullptr != m_pDiffTexture) m_pDiffTexture->AddRefCnt();
    if (nullptr != m_pNormalTexture) m_pNormalTexture->AddRefCnt();
    if (nullptr != m_pShader) m_pShader->AddRefCnt();

    m_pBoundingBox = CBoundingBox::Create(
        rhs.m_pBoundingBox->m_vMin
        , rhs.m_pBoundingBox->m_vMax
        , rhs.m_pBoundingBox->m_shaderID);

    m_pTriangles = new TRIANGLE[rhs.m_iTriNum];
    memcpy(m_pTriangles, rhs.m_pTriangles, sizeof(TRIANGLE) * rhs.m_iTriNum);
;}

CMesh::~CMesh()
{
}

// Basic Render Function, render 3D mesh with shader/texture information
void CMesh::Render()
{
    if (nullptr == m_pParentTransform || nullptr == m_pShader)
        return;
    
    mat4x4 matWorld = *m_pParentTransform->GetWorldMatrix();
    const mat4x4 matView = m_pOpenGLDevice->GetViewMatrix();
    const mat4x4 matProj = m_pOpenGLDevice->GetProjMatrix();

    if (!m_bBiilboard)
    {
        //if (nullptr != m_pAnimController)
        //{
        //    mat4x4 matAnim = m_pAnimController->GetMatrix();
        //    vec3 vScale = m_pParentTransform->GetScale();
        //    vScale.x = 1.f / vScale.x;
        //    vScale.y = 1.f / vScale.y;
        //    vScale.z = 1.f / vScale.z;
        //    matAnim[3][0] *= vScale.x;
        //    matAnim[3][1] *= vScale.y;
        //    matAnim[3][2] *= vScale.z;
        //    matWorld = matWorld * matAnim;
        //}

        m_pShader->SetMatrixInfo(matWorld, matView, matProj);
    }
    else
    {
        //mat4x4 newView = mat4x4(matView);
        //newView[3] = vec4(0.f);
        //mat4x4 matViewInv = inverse(matView);
        //matViewInv[3] = matWorld[3];
        //vec3 vScale = m_pParentTransform->GetScale();
        //for (int i = 0; i < 3; ++i)
        //{
        //    matViewInv[0][i] *= vScale.x;
        //    matViewInv[1][i] *= vScale.y;
        //    matViewInv[2][i] *= vScale.z;
        //}
        //m_pShader->SetMatrixInfo(matViewInv, matView, matProj);
        m_pShader->SetMatrixInfo(matWorld, matView, matProj);
    }

    m_pShader->SetLightEnableInfo(!m_bWireFrame);
    m_pShader->SetSelected(m_bSelected);
    m_pShader->SetTransparency(m_bTransparency);

    if (nullptr != m_pDiffTexture)
    {
        m_pShader->SetTextureInfo();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_pDiffTexture->GetTextureID());
    }

    if (nullptr != m_pNormalTexture)
    {
        m_pShader->SetNormalTextureInfo();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_pNormalTexture->GetTextureID());
    }

	if (nullptr != m_pVIBuffer)
    {
        if (m_bTransparency)
            glDepthMask(GL_FALSE);
        else
            glDepthMask(GL_TRUE);
        m_pVIBuffer->SetWireFrame(m_bWireFrame);
        m_pVIBuffer->Render();

        glDepthMask(GL_TRUE);
    }

    if (m_bDebug && nullptr != m_pBoundingBox)
        m_pBoundingBox->Render();
}

// Call instead of destructor to manage class internal data
void CMesh::Destroy()
{
    SafeDestroy(m_pOpenGLDevice);
    SafeDestroy(m_pVIBuffer);
    SafeDestroy(m_pBoundingBox);
    SafeDestroy(m_pDiffTexture);
    SafeDestroy(m_pNormalTexture);
    SafeDestroy(m_pShader);
    m_pParentTransform = nullptr;
    if (nullptr != m_pTriangles)
        delete m_pTriangles;

	CComponent::Destroy();
}

// Set diffuse texture
void CMesh::SetTexture(std::string texID_diff)
{
    SafeDestroy(m_pDiffTexture);

    CComponent* pComponent = CloneComponent<CTexture*>(texID_diff);
    if (nullptr != pComponent)
        m_pDiffTexture = dynamic_cast<CTexture*>(pComponent);
}

// Initialize Mesh
RESULT CMesh::Ready(string ID, string filePath, string fileName, eModelType type,
    string shaderID, string initSize, string meshType, string texID_Diff, string texID_Normal)
{
    m_tag = ID;
    m_initSize = initSize;
    m_meshType = meshType;

    VTX* pVertices = nullptr;
    _uint* pIndices = nullptr;
    _uint vertexNum = 0;
    _uint indexNum = 0;

    Ready_VIBuffer(type, filePath, fileName, &pVertices, &pIndices, vertexNum, indexNum);

    m_pVIBuffer = CVIBuffer::Create(vertexNum, pVertices, indexNum, pIndices, type);
    
    delete[] pVertices;
    delete[] pIndices;

    Ready_Texture_Diff(texID_Diff);
    Ready_Texture_Normal(texID_Normal);
    Ready_Shader(shaderID);

    if (nullptr != m_pShader)
        m_pShader->SetTextureInfo();

	return PK_NOERROR;
}

// Load mesh information from file
RESULT CMesh::Ready_VIBuffer(eModelType type, string filePath, string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum)
{
    stringstream ss;
    ss << filePath << fileName;
    ifstream file(ss.str());
    if (!file.is_open())
        return PK_ERROR_MESHFILE_OPEN;

    _uint triangleNum = 0;
    const _uint bufSize = 10000;
    char buffer[bufSize];
    file.getline(buffer, bufSize);

    string next;
    while (file >> next)
    {
        if (next == "end_header")
            break;

        if (next == "vertex")
            file >> vertexNum;

        if (next == "face")
            file >> triangleNum;

        if (next == "TextureFile")
            file >> m_textureFileName;
    }

    vec3 vMin = vec3(FLT_MAX);
    vec3 vMax = vec3(FLT_MIN);
    _int r = GetRandNum(0, 170);
    _int g = GetRandNum(0, 170);
    _int b = GetRandNum(0, 170);
    vec3 rbg = vec3(r / 255.f, g / 255.f, b / 255.f);

    *pVertices = new VTX[vertexNum];
    memset(*pVertices, 0, sizeof(**pVertices));
    for (_uint i = 0; i < vertexNum; ++i)
    {
        vec4& vPos = (*pVertices)[i].vPos;
        file >> vPos.x;
        file >> vPos.y;
        file >> vPos.z;
        vPos.w = 1.f;

        switch (type)
        {
        case xyz_index:
            (*pVertices)[i].vNormal = vec4(0.f, 1.f, 0.f, 1.f);
            break;

        case xyz_normal_index:
        case xyz_normal_color_index:
            file >> (*pVertices)[i].vNormal.x;
            file >> (*pVertices)[i].vNormal.y;
            file >> (*pVertices)[i].vNormal.z;
            (*pVertices)[i].vNormal.w = 1.f;
            break;

        case xyz_normal_texUV_index:
        case xyz_normal_texUV_index_texNum:
            file >> (*pVertices)[i].vNormal.x;
            file >> (*pVertices)[i].vNormal.y;
            file >> (*pVertices)[i].vNormal.z;
            (*pVertices)[i].vNormal.w = 1.f;
            file >> (*pVertices)[i].vTexUV.x;
            file >> (*pVertices)[i].vTexUV.y;
            break;
        }

        if (type == xyz_normal_color_index)
        {
            file >> (*pVertices)[i].vColour.x; (*pVertices)[i].vColour.x = (*pVertices)[i].vColour.x / 255.f;
            file >> (*pVertices)[i].vColour.y; (*pVertices)[i].vColour.y = (*pVertices)[i].vColour.y / 255.f;
            file >> (*pVertices)[i].vColour.z; (*pVertices)[i].vColour.z = (*pVertices)[i].vColour.z / 255.f;
            file >> (*pVertices)[i].vColour.w; (*pVertices)[i].vColour.w = (*pVertices)[i].vColour.w / 255.f;
        }
        else
        {
            (*pVertices)[i].vColour.r = rbg.r;
            (*pVertices)[i].vColour.g = rbg.g;
            (*pVertices)[i].vColour.b = rbg.b;
            (*pVertices)[i].vColour.a = 1.0f;
        }

        if (vMin.x > vPos.x)
            vMin.x = vPos.x;
        if (vMin.y > vPos.y)
            vMin.y = vPos.y;
        if (vMin.z > vPos.z)
            vMin.z = vPos.z;
        if (vMax.x < vPos.x)
            vMax.x = vPos.x;
        if (vMax.y < vPos.y)
            vMax.y = vPos.y;
        if (vMax.z < vPos.z)
            vMax.z = vPos.z;
    }
    
    m_pBoundingBox = CBoundingBox::Create(vMin, vMax, "DebugBoxShader");

    m_iTriNum = triangleNum;
    m_pTriangles = new TRIANGLE[triangleNum];
    memset(m_pTriangles, 0, sizeof(*m_pTriangles));

    IDX* pTriangles = new IDX[triangleNum];
    memset(pTriangles, 0, sizeof(*pTriangles));
    int discard = 0;
    for (_uint i = 0; i < triangleNum; ++i)
    {
        file >> discard;
        file >> pTriangles[i]._0;
        file >> pTriangles[i]._1;
        file >> pTriangles[i]._2;
        if (type == xyz_normal_texUV_index_texNum)
            file >> discard;

        m_pTriangles[i].p0 = (*pVertices)[pTriangles[i]._0].vPos;
        m_pTriangles[i].p1 = (*pVertices)[pTriangles[i]._1].vPos;
        m_pTriangles[i].p2 = (*pVertices)[pTriangles[i]._2].vPos;
    }
    file.close();

    indexNum = triangleNum * 3;
    *pIndices = new _uint[indexNum];
    memset(*pIndices, 0, sizeof(**pIndices));
    _uint indexCount = 0;
    for (_uint i = 0; i < triangleNum; ++i)
    {
        (*pIndices)[indexCount + 0] = pTriangles[i]._0;
        (*pIndices)[indexCount + 1] = pTriangles[i]._1;
        (*pIndices)[indexCount + 2] = pTriangles[i]._2;

        indexCount += 3;
    }

    delete[] pTriangles;

    return PK_NOERROR;
}

// Initialize diffuse texture
void CMesh::Ready_Texture_Diff(string texID)
{
    CComponent* pComponent = CloneComponent<CTexture*>(texID);
    if (nullptr != pComponent)
        m_pDiffTexture = dynamic_cast<CTexture*>(pComponent);
}

// Initialize normal texture
void CMesh::Ready_Texture_Normal(std::string texID)
{
    CComponent* pComponent = CloneComponent<CTexture*>(texID);
    if (nullptr != pComponent)
        m_pNormalTexture = dynamic_cast<CTexture*>(pComponent);
}

// Initialize shader
void CMesh::Ready_Shader(string shaderID)
{
    CComponent* pComponent = CloneComponent<CShader*>(shaderID);
    if (nullptr != pComponent)
        m_pShader = dynamic_cast<CShader*>(pComponent);
}

//void CMesh::Ready_QuadTree(_uint depth)
//{
    //if (nullptr == m_pBoundingBox || nullptr == m_pTriangles)
    //    return;

    //m_pQuadTree = CQuadTree::Create(m_pBoundingBox->m_vMax, m_pBoundingBox->m_vMin, depth);
    //if (nullptr == m_pQuadTree)
    //    return;

    //for (_uint i = 0; i < m_iTriNum; ++i)
    //{
    //    _int hashKey = m_pQuadTree->GetHashValue(m_pTriangles[i].p0.x, m_pTriangles[i].p0.z);
    //    m_pQuadTree->AddTriangleToTreeNode(hashKey, m_pTriangles[i]);

    //    hashKey = m_pQuadTree->GetHashValue(m_pTriangles[i].p1.x, m_pTriangles[i].p1.z);
    //    m_pQuadTree->AddTriangleToTreeNode(hashKey, m_pTriangles[i]);

    //    hashKey = m_pQuadTree->GetHashValue(m_pTriangles[i].p2.x, m_pTriangles[i].p2.z);
    //    m_pQuadTree->AddTriangleToTreeNode(hashKey, m_pTriangles[i]);
    //}
//}

//void CMesh::Ready_Qctree(_uint depth)
//{
//    if (nullptr == m_pBoundingBox || nullptr == m_pTriangles)
//        return;
//
//    m_pOctree = COctree::Create(m_pBoundingBox->m_vMax, m_pBoundingBox->m_vMin, depth);
//    if (nullptr == m_pOctree)
//        return;
//
//    _uint missed = 0;
//    for (_uint i = 0; i < m_iTriNum; ++i)
//    {
//        m_pOctree->AddTriangle(m_pTriangles[i], missed);
//    }
//}

// Clone component
CComponent* CMesh::Clone()
{
    return new CMesh(*this);
}

// Create an instance
CMesh* CMesh::Create(string ID, string filePath, string fileName, eModelType type,
    string shaderID, string initSize, string meshType, string texID_Diff, string texID_Normal)
{
	CMesh* pInstance = new CMesh();
	if (PK_NOERROR != pInstance->Ready(ID, filePath, fileName, type,
        shaderID, initSize, meshType,
        texID_Diff, texID_Normal))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

