#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include "Base.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

NAMESPACE_BEGIN(Engine)

class CComponent;

// Save/Load XML files
class ENGINE_API CXMLParser : public CBase
{
	SINGLETON(CXMLParser)

	struct sShaderdata
	{
		std::string ID;
		std::string PATH_VERTEX;
		std::string PATH_FRAGMENT;
	};

	struct sTexturedata
	{
		std::string ID;
		std::string PATH;
	};

	struct sMeshdata
	{
		std::string ID;
		std::string PATH;
		std::string FILENAME;
		_uint TYPE;
		std::string SHADER_ID;
		std::string TEXTURE_ID_DIFF;
		std::string INITSIZE;
	};

	struct sObjectData
	{
		std::string ID;
		glm::vec3 POSITION;
		glm::vec3 ROTATION;
		glm::vec3 SCALE;
		std::string SOUNDTAG;
		_bool LOCK;
	};

	struct sLightData
	{
		std::string NAME;
		glm::vec4 POSITION;
		glm::vec4 DIRECTION;
		glm::vec4 DIFFUSE;
		glm::vec4 SPECULAR;
		glm::vec4 AMBIENT;
		glm::vec4 ATTEN;
		glm::vec4 PARAM1;
		glm::vec4 PARAM2;
	};

private:
	std::string				m_xmlDataPath;

private:
	explicit CXMLParser();
	virtual ~CXMLParser();
private:
	void Destroy();

public:
	void LoadShaderData(std::string path, std::string fileName);
	void LoadTextureData(std::string path, std::string fileName);
	void LoadMeshData(std::string path, std::string fileName);
	void LoadSoundData(std::string path, std::string fileName);
	void LoadMapObjectData(std::string path, std::string fileName, std::vector<sObjectData>& vec, sObjectData& cameraData);
	void SaveMapObjectData(std::string path, std::string fileName, std::vector<sObjectData>& vec, sObjectData& cameraData);
	void LoadLightData(std::string path, std::string fileName, std::vector<sLightData>& vec);
	void SaveLightData(std::string path, std::string fileName, std::vector<sLightData>& vec);
	void LoadLanguageData(std::string path, std::string fileName, std::unordered_map<std::string, std::string>& map);
};

NAMESPACE_END

#endif //_XMLPARSER_H_