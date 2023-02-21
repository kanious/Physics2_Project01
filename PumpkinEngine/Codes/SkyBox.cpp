#include "pch.h"
#include "../Headers/SkyBox.h"
#include "../Headers/OpenGLDevice.h"
#include "../Headers/Shader.h"
#include "../Headers/Transform.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "stb_image.h"


USING(Engine)
USING(std)
USING(glm)

CSkyBox::CSkyBox()
    : m_pOpenGLDevice(COpenGLDevice::GetInstance()), m_pShader(nullptr)
    , m_iTextureID(0), m_iVAO(0), m_iVBO(0)
{
    m_pOpenGLDevice->AddRefCnt();
}

CSkyBox::~CSkyBox()
{

}

// Basic Render Function
void CSkyBox::Render()
{
    if (nullptr == m_pShader || nullptr == m_pTransform)
        return;

    glDepthMask(GL_FALSE);

    const mat4x4 matWorld = mat4x4(1.f);
    mat4x4 matView = mat4x4(mat3(m_pOpenGLDevice->GetViewMatrix()));
    const mat4x4 matProj = m_pOpenGLDevice->GetProjMatrix();
    m_pShader->SetMatrixInfo(matWorld, matView, matProj);

    glBindVertexArray(m_iVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_iTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

// Call instead of destructor to manage class internal data
void CSkyBox::Destroy()
{
    SafeDestroy(m_pOpenGLDevice);
    glDeleteVertexArrays(1, &m_iVAO);
    glDeleteBuffers(1, &m_iVBO);
    glDeleteTextures(2, &m_iTextureID);

    CGameObject::Destroy();
}

// Initialize Skybox
RESULT CSkyBox::Ready(vector<string>& faces, CShader* pShader)
{
    m_pShader = pShader;

    Ready_Texture(faces);
    Ready_VIBuffer();

    return PK_NOERROR;
}

// Load Skybox Textures
RESULT CSkyBox::Ready_Texture(vector<string>& faces)
{
    glGenTextures(1, &m_iTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_iTextureID);

    int nrChannels;
    for (int i = 0; i < faces.size(); ++i)
    {
        int width, height;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return PK_NOERROR;
}

// Load Skybox Mesh (Cube)
RESULT CSkyBox::Ready_VIBuffer()
{
    _float skyboxVertices[] = {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_iVAO);
    glGenBuffers(1, &m_iVBO);
    glBindVertexArray(m_iVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(_float), (void*)0);

	return PK_NOERROR;
}

// Create an instance
CSkyBox* CSkyBox::Create(vector<string>& faces, CShader* pShader)
{
    CSkyBox* pInstance = new CSkyBox();
    if (PK_NOERROR != pInstance->Ready(faces, pShader))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
