#include "pch.h"
#include "..\Headers\JsonParser.h"
#include "..\Headers\ComponentMaster.h"
#include "..\Headers\Mesh.h"
#include "..\Headers\Texture.h"
#include "glm\vec3.hpp"

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"

#include <sstream>


USING(Engine)
USING(std)
USING(glm)
USING(rapidjson)
SINGLETON_FUNCTION(CJsonParser)

CJsonParser::CJsonParser()
{
	m_jsonDataPath = "Json\\";
	m_pCompMaster = CComponentMaster::GetInstance(); m_pCompMaster->AddRefCnt();
}

CJsonParser::~CJsonParser()
{
}

// Call instead of destructor to manage class internal data
void CJsonParser::Destroy()
{
	SafeDestroy(m_pCompMaster);
}

// Load Characters from file
void CJsonParser::LoadCharacterList(std::string assetFolderPath, std::string fileName, std::vector<sCharacterData>& vec)
{
	Document doc;
	FILE* file;
	LoadDataFromFile(doc, file, assetFolderPath, fileName);
	const Value& dataArray = doc["array"];
	assert(dataArray.IsArray());

	for (int i = 0; i < dataArray.Size(); ++i)
	{
		sCharacterData data;

		const Value& curData = dataArray[i];
		data.MESHID = curData["meshname"].GetString();
		
		data.POSITION.x = curData["posx"].GetFloat();
		data.POSITION.y = curData["posy"].GetFloat();
		data.POSITION.z = curData["posz"].GetFloat();

		data.ROTATION.x = curData["rotx"].GetFloat();
		data.ROTATION.y = curData["roty"].GetFloat();
		data.ROTATION.z = curData["rotz"].GetFloat();

		data.SCALE.x = curData["scalex"].GetFloat();
		data.SCALE.y = curData["scaley"].GetFloat();
		data.SCALE.z = curData["scalez"].GetFloat();

		data.ANIMTYPE = curData["animtype"].GetInt();
		data.EASETYPE = curData["easetype"].GetInt();
		data.ANIMRANDOM = curData["random"].GetBool();

		vec.push_back(data);
	}

	std::fclose(file);
}

// Load Textures from file
void CJsonParser::LoadTextureData(std::string assetFolderPath, std::string fileName)
{
	Document doc;
	FILE* file;
	LoadDataFromFile(doc, file, assetFolderPath, fileName);

	for (unsigned int i = 0; i < doc.Size(); ++i)
	{
		sTexturedata data;

		const Value& curData = doc[i];
		data.ID = curData["ID"].GetString();
		data.PATH = curData["Path"].GetString();
		data.FILENAME = curData["FileName"].GetString();

		stringstream ss;
		ss << assetFolderPath << data.PATH << data.FILENAME;
		CComponent* pComp = CTexture::Create(data.ID, ss.str());

		if (nullptr != pComp)
			m_pCompMaster->AddNewComponent(data.ID, pComp);

		cout << "Texture Loading... " << data.ID << " Loaded" << endl;
	}

	std::fclose(file);
}

// Load Meshes from file
void CJsonParser::LoadMeshData(std::string assetFolderPath, std::string fileName, _bool saveMeshList)
{
	Document doc;
	FILE* file;
	LoadDataFromFile(doc, file, assetFolderPath, fileName);

	for (unsigned int i = 0; i < doc.Size(); ++i)
	{
		sMeshData data;

		const Value& curData = doc[i];

		data.ID = curData["ID"].GetString();
		data.PATH = curData["Path"].GetString();
		data.FILENAME = curData["FileName"].GetString();
		data.DATATYPE = curData["DataType"].GetInt();
		data.SHADER_ID = curData["Shader_ID"].GetString();
		data.INITSIZE = to_string(curData["InitSize"].GetFloat());
		data.MESHTYPE = curData["MeshType"].GetString();
		data.TEXTURE_ID_DIFF = curData["Texture_ID_Diff"].GetString();
		data.TEXTURE_ID_NORMAL = curData["Texture_ID_Normal"].GetString();

		stringstream ss;
		ss << assetFolderPath << data.PATH;
		CComponent* pComp = CMesh::Create(data.ID, ss.str(), data.FILENAME,
			(eModelType)data.DATATYPE, data.SHADER_ID, data.INITSIZE, data.MESHTYPE,
			data.TEXTURE_ID_DIFF, data.TEXTURE_ID_NORMAL);

		if (nullptr != pComp)
		{
			m_pCompMaster->AddNewComponent(data.ID, pComp);
			if (saveMeshList)
				m_pCompMaster->AddNewMeshInfo(data.ID);
		}

		cout << "Mesh Loading... " << data.ID << " Loaded" << endl;
	}

	std::fclose(file);
}

// Load Objects from file
void CJsonParser::LoadObjectList(string assetFolderPath, string fileName, vector<sObjectData>& vec, sObjectData& cameraData)
{
	Document doc;
	FILE* file;
	LoadDataFromFile(doc, file, assetFolderPath, fileName);

	for (unsigned int i = 0; i < doc.Size(); ++i)
	{
		sObjectData data;

		const Value& curData = doc[i];

		data.ID = curData["modelName"].GetString();
		data.LAYERTYPE = curData["layerType"].GetString();
		data.POSITION.x = curData["posX"].GetFloat();
		data.POSITION.y = curData["posY"].GetFloat();
		data.POSITION.z = curData["posZ"].GetFloat();
		data.ROTATION.x = curData["rotX"].GetFloat();
		data.ROTATION.y = curData["rotY"].GetFloat();
		data.ROTATION.z = curData["rotZ"].GetFloat();
		data.SCALE.x = curData["scaleX"].GetFloat();
		data.SCALE.y = curData["scaleY"].GetFloat();
		data.SCALE.z = curData["scaleZ"].GetFloat();
		data.LOCK = curData["lock"].GetBool();
		data.NORMAL.x = curData["normalX"].GetFloat();
		data.NORMAL.y = curData["normalY"].GetFloat();
		data.NORMAL.z = curData["normalZ"].GetFloat();
		data.ISGROUND = curData["IsGround"].GetBool();

		if (!strcmp("camera", data.LAYERTYPE.c_str()))
			cameraData = data;
		else
			vec.push_back(data);
	}

	std::fclose(file);
}

// Save Objects from file
void CJsonParser::SaveObjectList(string assetFolderPath, string fileName, vector<sObjectData>& vec, sObjectData& cameraData)
{
	stringstream ss;
	ss << assetFolderPath << m_jsonDataPath << fileName;

	FILE* file;
	fopen_s(&file, ss.str().c_str(), "wb");
	const _uint BUFFER_SIZE = 1000;
	char writeBuffer[BUFFER_SIZE];
	FileWriteStream output(file, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(output);


	char buffer[BUFFER_SIZE];
	int len = 0;

	Document doc;
	doc.SetArray();
	Document::AllocatorType& allocator = doc.GetAllocator();

	// Camera
	{
		Value obj;
		obj.SetObject();

		Value id;
		len = sprintf_s(buffer, "%s", "Camera");
		id.SetString(buffer, len, allocator);
		obj.AddMember("modelName", id, allocator);

		Value layerType;
		len = sprintf_s(buffer, "%s", "Camera");
		layerType.SetString(buffer, len, allocator);
		obj.AddMember("layerType", layerType, allocator);

		Value posX;	posX.SetFloat(cameraData.POSITION.x); obj.AddMember("posX", posX, allocator);
		Value posY;	posY.SetFloat(cameraData.POSITION.y); obj.AddMember("posY", posY, allocator);
		Value posZ; posZ.SetFloat(cameraData.POSITION.z); obj.AddMember("posZ", posZ, allocator);

		Value rotX;	rotX.SetFloat(cameraData.ROTATION.x); obj.AddMember("rotX", rotX, allocator);
		Value rotY;	rotY.SetFloat(cameraData.ROTATION.y); obj.AddMember("rotY", rotY, allocator);
		Value rotZ; rotZ.SetFloat(cameraData.ROTATION.z); obj.AddMember("rotZ", rotZ, allocator);

		Value scaleX; scaleX.SetFloat(cameraData.SCALE.x); obj.AddMember("scaleX", scaleX, allocator);
		Value scaleY; scaleY.SetFloat(cameraData.SCALE.y); obj.AddMember("scaleY", scaleY, allocator);
		Value scaleZ; scaleZ.SetFloat(cameraData.SCALE.z); obj.AddMember("scaleZ", scaleZ, allocator);

		Value lock; lock.SetBool(false); obj.AddMember("lock", lock, allocator);

		doc.PushBack(obj, allocator);
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		sObjectData curData = vec[i];

		Value obj;
		obj.SetObject();

		Value id;
		len = sprintf_s(buffer, "%s", curData.ID.c_str());
		id.SetString(buffer, len, allocator);
		obj.AddMember("modelName", id, allocator);

		Value layerType;
		len = sprintf_s(buffer, "%s", curData.LAYERTYPE.c_str());
		layerType.SetString(buffer, len, allocator);
		obj.AddMember("layerType", layerType, allocator);

		Value posX;	posX.SetFloat(curData.POSITION.x); obj.AddMember("posX", posX, allocator);
		Value posY;	posY.SetFloat(curData.POSITION.y); obj.AddMember("posY", posY, allocator);
		Value posZ; posZ.SetFloat(curData.POSITION.z); obj.AddMember("posZ", posZ, allocator);

		Value rotX;	rotX.SetFloat(curData.ROTATION.x); obj.AddMember("rotX", rotX, allocator);
		Value rotY;	rotY.SetFloat(curData.ROTATION.y); obj.AddMember("rotY", rotY, allocator);
		Value rotZ; rotZ.SetFloat(curData.ROTATION.z); obj.AddMember("rotZ", rotZ, allocator);

		Value scaleX; scaleX.SetFloat(curData.SCALE.x); obj.AddMember("scaleX", scaleX, allocator);
		Value scaleY; scaleY.SetFloat(curData.SCALE.y); obj.AddMember("scaleY", scaleY, allocator);
		Value scaleZ; scaleZ.SetFloat(curData.SCALE.z); obj.AddMember("scaleZ", scaleZ, allocator);

		Value lock; lock.SetBool(curData.LOCK); obj.AddMember("lock", lock, allocator);

		doc.PushBack(obj, allocator);
	}

	doc.Accept(writer);
	std::fclose(file);
}

// Open files for loading
void CJsonParser::LoadDataFromFile(Document& doc, FILE*& file, string assetFolderPath, string fileName)
{
	stringstream ss;
	ss << assetFolderPath << m_jsonDataPath << fileName;

	fopen_s(&file, ss.str().c_str(), "rb");
	const _uint BUFFER_SIZE = 1000;
	char readBuffer[BUFFER_SIZE];
	FileReadStream input(file, readBuffer, sizeof(readBuffer));
	doc.ParseStream(input);
}
