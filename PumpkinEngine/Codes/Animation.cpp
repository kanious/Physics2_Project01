#include "pch.h"
#include "..\Headers\Animation.h"
#include "..\Headers\OpenGLDefines.h"
#include <windows.h>


USING(Engine)
USING(std)
USING(glm)

CAnimation::CAnimation()
    : m_tag(""), m_iDuration(0), m_fTickPerSec(0.f)
{
    m_vecMatrix.clear();
}

CAnimation::~CAnimation()
{
}

void CAnimation::Destroy()
{
    m_vecMatrix.clear();
}

void CAnimation::FrameMove(_float& curTime, _uint& frameIdx, _bool reverse, function<void(void)> callback)
{
    if (curTime >= m_fTickPerSec)
    {
        curTime = 0.f;
        if (!reverse)
        {
            if (frameIdx >= m_vecMatrix.size() - 1)
            {
                frameIdx = 0;
                callback();
            }
            else
                ++frameIdx;
        }
        else
        {
            if (0 == frameIdx)
            {
                frameIdx = m_vecMatrix.size() - 1;
                callback();
            }
            else
                --frameIdx;
        }
    }
}

mat4x4 CAnimation::GetMatrix(_uint iFrameIndex)
{
    return m_vecMatrix[iFrameIndex];
}

RESULT CAnimation::Ready(string tag, string filePath)
{
    wstring wstr = wstring(filePath.begin(), filePath.end());
    HANDLE hFile = CreateFile(wstr.c_str()
        , GENERIC_READ
        , 0
        , NULL
        , OPEN_EXISTING
        , FILE_ATTRIBUTE_NORMAL
        , NULL);
    unsigned long dwByte;

    bool success = false;
    success = ReadFile(hFile, &m_iDuration, sizeof(_uint), &dwByte, NULL);
    success = ReadFile(hFile, &m_fTickPerSec, sizeof(_float), &dwByte, NULL);

    for (int i = 0; i < m_iDuration; ++i)
    {
        mat4x4 mat(1.f);
        success = ReadFile(hFile, &mat, sizeof(mat4x4), &dwByte, NULL);
        m_vecMatrix.push_back(mat);
    }

    return PK_NOERROR;
}

CAnimation* CAnimation::Create(string tag, string filePath)
{
    CAnimation* pInstance = new CAnimation();
    if (PK_NOERROR != pInstance->Ready(tag, filePath))
    {
        pInstance->Destroy();
        pInstance = nullptr;
    }

    return pInstance;
}
