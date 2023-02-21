#include "pch.h"
#include "../Headers/Texture.h"
#include "../Headers/OpenGLDefines.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>

USING(Engine)
USING(std)

CTexture::CTexture()
    : m_iTextureID(0), m_iWidth(0), m_iHeight(0)
{
}

CTexture::CTexture(const CTexture& rhs)
    : m_iTextureID(rhs.m_iTextureID), m_iWidth(rhs.m_iWidth), m_iHeight(rhs.m_iHeight)
{
    m_tag = rhs.m_tag;
}

CTexture::~CTexture()
{
}

// Call instead of destructor to manage class internal data
void CTexture::Destroy()
{
    glDeleteTextures(1, &m_iTextureID);

    CComponent::Destroy();
}

// Load texture information from file
RESULT CTexture::Ready(string ID, string filePath)
{
    m_tag = ID;

    glGenTextures(1, &m_iTextureID);
    glBindTexture(GL_TEXTURE_2D, m_iTextureID);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &m_iWidth, &m_iHeight, &nrChannels, 0);

    if (data)
    {
        if (!strcmp("cemetery_halloween_Tex", ID.c_str()))
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    
    return PK_NOERROR;
}

// Clone component
CComponent* CTexture::Clone()
{
    return new CTexture(*this);
}

// Create an instance
CTexture* CTexture::Create(string ID, string filePath)
{
    CTexture* pInstance = new CTexture();
    if (PK_NOERROR != pInstance->Ready(ID, filePath))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
