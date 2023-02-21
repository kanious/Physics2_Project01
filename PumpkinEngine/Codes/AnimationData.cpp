#include "pch.h"
#include "..\Headers\AnimationData.h"
#include "..\Headers\Animation.h"
#include <fstream>


USING(Engine)
USING(std)
SINGLETON_FUNCTION(CAnimationData)

CAnimationData::CAnimationData()
{
}

CAnimationData::~CAnimationData()
{
}

void CAnimationData::Destroy()
{
	ANIM_MAP::iterator iter;
	for (iter = m_mapAnimations.begin(); iter != m_mapAnimations.end(); ++iter)
		SafeDestroy(iter->second);
	m_mapAnimations.clear();
}

void CAnimationData::AddAnimation(string tag, CAnimation* pAnimation)
{
	ANIM_MAP::iterator iter = m_mapAnimations.find(tag);
	if (iter == m_mapAnimations.end())
		m_mapAnimations.insert(ANIM_MAP::value_type(tag, pAnimation));
}

CAnimation* CAnimationData::FindAnimation(string tag)
{
	ANIM_MAP::iterator iter = m_mapAnimations.find(tag);
	if (iter != m_mapAnimations.end())
		return iter->second;

	return nullptr;
}

void CAnimationData::LoadAnimations(string assetFolderPath)
{
	string listFilePath = assetFolderPath + "Animation\\animList.txt";

	ifstream file(listFilePath.c_str());
	if (!file.is_open())
		return;

	const _uint BUFFER_SIZE = 1000;
	char textBuffer[BUFFER_SIZE];
	_bool keepReading = true;

	while (keepReading)
	{
		file.getline(textBuffer, BUFFER_SIZE);

		if (!strcmp("EOF", textBuffer))
		{
			keepReading = false;
			continue;
		}

		string tag(textBuffer);
		string animPath = assetFolderPath + "Animation\\" + tag + ".animation";
		CAnimation* pAnim = CAnimation::Create(tag, animPath);
		if (nullptr != pAnim)
			AddAnimation(tag, pAnim);
	}
}
