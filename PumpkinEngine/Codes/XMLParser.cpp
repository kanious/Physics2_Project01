#include "pch.h"
#include "..\Headers\XMLParser.h"
#include "..\Headers\ComponentMaster.h"
#include "..\Headers\Component.h"
#include "..\Headers\Mesh.h"
#include "..\Headers\Shader.h"
#include "..\Headers\Texture.h"
#include "..\Headers\SoundMaster.h"
#include <pugixml/pugixml.hpp>
#include <sstream>


USING(Engine)
USING(pugi)
USING(std)
SINGLETON_FUNCTION(CXMLParser)

CXMLParser::CXMLParser()
{
	m_xmlDataPath = "xmlData\\";
}

CXMLParser::~CXMLParser()
{
}

// Call instead of destructor to manage class internal data
void CXMLParser::Destroy()
{
}

// Load Shaders from file
void CXMLParser::LoadShaderData(string path, string fileName)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;
	
	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Shader_List").children();
	if (list.empty())
		return;

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;
	
	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_node_iterator childIter;

		sShaderdata data;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(childNode.name(), "ID"))
				data.ID = childNode.child_value();
			if (!strcmp(childNode.name(), "VertexPath"))
				data.PATH_VERTEX = childNode.child_value();
			if (!strcmp(childNode.name(), "FragmentPath"))
				data.PATH_FRAGMENT= childNode.child_value();
		}
		
		ss.str("");
		ss << path << data.PATH_VERTEX;
		stringstream ss2;
		ss2 << path << data.PATH_FRAGMENT;
		pComponent = CShader::Create(data.ID, ss.str().c_str(), ss2.str().c_str());
		if (nullptr != pComponent)
			pMaster->AddNewComponent(data.ID, pComponent);
	}
}

// Load Textures from file
void CXMLParser::LoadTextureData(string path, string fileName)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;
	
	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Texture_List").children();
	if (list.empty())
		return;

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_node_iterator childIter;

		sTexturedata data;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(childNode.name(), "ID"))
				data.ID = childNode.child_value();
			if (!strcmp(childNode.name(), "Path"))
				data.PATH = childNode.child_value();
		}

		stringstream ss;
		ss << path << data.PATH;
		pComponent = CTexture::Create(data.ID, ss.str());
		if (nullptr != pComponent)
			pMaster->AddNewComponent(data.ID, pComponent);
	}
}

// Load Meshes from file
void CXMLParser::LoadMeshData(string path, string fileName)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;

	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Mesh_List").children();
	if (list.empty())
		return;

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_node_iterator childIter;
		
		sMeshdata data;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(childNode.name(), "ID"))
				data.ID = childNode.child_value();
			if (!strcmp(childNode.name(), "Path"))
				data.PATH = childNode.child_value();
			if (!strcmp(childNode.name(), "FileName"))
				data.FILENAME = childNode.child_value();
			if (!strcmp(childNode.name(), "Type"))
				data.TYPE = (_uint)atoi(childNode.child_value());
			if (!strcmp(childNode.name(), "Shader_ID"))
				data.SHADER_ID = childNode.child_value();
			if (!strcmp(childNode.name(), "Texture_ID_Diff"))
				data.TEXTURE_ID_DIFF = childNode.child_value();
			if (!strcmp(childNode.name(), "InitSize"))
				data.INITSIZE = childNode.child_value();
		}

		stringstream ss;
		ss << path << data.PATH;
		pComponent = CMesh::Create(data.ID, ss.str(), data.FILENAME, (eModelType)data.TYPE,
			data.SHADER_ID, "", "", data.TEXTURE_ID_DIFF, "");
		if (nullptr != pComponent)
		{
			pMaster->AddNewComponent(data.ID, pComponent);
			pMaster->AddNewMeshInfo(data.ID);
		}
	}
}

// Load Sounds from file
void CXMLParser::LoadSoundData(string path, string fileName)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;
	
	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Sound_Data").children();
	if (list.empty())
		return;

	CSoundMaster* pInstance = CSoundMaster::GetInstance();
	_bool compressed = pInstance->GetCompressed();
	pInstance->SetDataPath(path);
	pInstance->SetFileName(fileName);

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_attribute category = node.attribute("category");
		xml_node_iterator childIter;

		_int channelCount = 0;
		string channelTag = "";
		string soundTag = "";
		string soundPath = "";
		string soundChannelTag = "";
		_bool loop = false;
		string dspTag = "";
		_int type = 0;

 		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(category.value(), "MaxChannelCount"))
			{
				if (!strcmp(childNode.name(), "Count"))
					channelCount = (_int)atoi(childNode.child_value());
			}
			else if (!strcmp(category.value(), "ChannelGroup"))
			{
				if (!strcmp(childNode.name(), "Tag"))
					channelTag = childNode.child_value();
			}
			else if (!strcmp(category.value(), "SoundFile"))
			{
				if (!strcmp(childNode.name(), "Tag"))
					soundTag = childNode.child_value();
				else if (!strcmp(childNode.name(), "Path"))
					soundPath = childNode.child_value();
				else if (!strcmp(childNode.name(), "ChannelTag"))
					soundChannelTag = childNode.child_value();
				else if (!strcmp(childNode.name(), "Loop"))
					loop = (_bool)atoi(childNode.child_value());
			}
			else if (!strcmp(category.value(), "DSP"))
			{
				if (!strcmp(childNode.name(), "Tag"))
					dspTag = childNode.child_value();
				else if (!strcmp(childNode.name(), "Type"))
					type = atoi(childNode.child_value());
			}
		}

		if (!strcmp(category.value(), "MaxChannelCount"))
			pInstance->Ready(channelCount);
		else if (!strcmp(category.value(), "ChannelGroup"))
			pInstance->CreateChannelGroup(channelTag);
		else if (!strcmp(category.value(), "SoundFile"))
		{
			// sound Path parcing
			//string token(soundPath);
			//size_t pos = token.find('.');
			//string command = token.substr(0, pos);
			//token.erase(0, command.size() + 1);

			//if (compressed)
			//{
			//	if (!strcmp("wav", token.c_str()))
			//		continue;
			//}
			//else
			//{
			//	if (!strcmp("mp3", token.c_str()))
			//		continue;
			//}

			stringstream ss;
			ss << path << soundPath;
			if (!loop)
				pInstance->LoadSound(soundTag, ss.str(), soundChannelTag);
			else
				pInstance->LoadLoopSound(soundTag, ss.str(), soundChannelTag);
			
		}
		else if (!strcmp(category.value(), "DSP"))
			pInstance->CreateDSPEffect(dspTag, type);
	}
}

// Load Objects from file
void CXMLParser::LoadMapObjectData(string path, string fileName, vector<sObjectData>& vec, sObjectData& cameraData)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;

	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Object_Data").children();
	if (list.empty())
		return;

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_attribute category = node.attribute("category");
		_bool isCamera = !strcmp(category.value(), "Camera");
		xml_node_iterator childIter;

		sObjectData data;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (isCamera)
			{
				if (!strcmp(childNode.name(), "EyePosX"))
					cameraData.POSITION.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "EyePosY"))
					cameraData.POSITION.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "EyePosZ"))
					cameraData.POSITION.z = (_float)atof(childNode.child_value());

				if (!strcmp(childNode.name(), "EyeRotX"))
					cameraData.ROTATION.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "EyeRotY"))
					cameraData.ROTATION.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "EyeRotZ"))
					cameraData.ROTATION.z = (_float)atof(childNode.child_value());

				if (!strcmp(childNode.name(), "TargetPosX"))
					cameraData.SCALE.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "TargetPosY"))
					cameraData.SCALE.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "TargetPosZ"))
					cameraData.SCALE.z = (_float)atof(childNode.child_value());
			}
			else
			{
				if (!strcmp(childNode.name(), "ID"))
					data.ID = childNode.child_value();
				if (!strcmp(childNode.name(), "PosX"))
					data.POSITION.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "PosY"))
					data.POSITION.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "PosZ"))
					data.POSITION.z = (_float)atof(childNode.child_value());

				if (!strcmp(childNode.name(), "RotX"))
					data.ROTATION.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "RotY"))
					data.ROTATION.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "RotZ"))
					data.ROTATION.z = (_float)atof(childNode.child_value());

				if (!strcmp(childNode.name(), "ScaleX"))
					data.SCALE.x = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "ScaleY"))
					data.SCALE.y = (_float)atof(childNode.child_value());
				if (!strcmp(childNode.name(), "ScaleZ"))
					data.SCALE.z = (_float)atof(childNode.child_value());

				if (!strcmp(childNode.name(), "EffectSound"))
					data.SOUNDTAG = childNode.child_value();
				if (!strcmp(childNode.name(), "Lock"))
					data.LOCK = (_bool)atoi(childNode.child_value());
			}
		}
		if (!isCamera)
			vec.push_back(data);
	}
}

// Save Objects from file
void CXMLParser::SaveMapObjectData(string path, string fileName, vector<sObjectData>& vec, sObjectData& cameraData)
{
	xml_document xmlData;

	xml_node declareNode = xmlData.append_child(node_declaration);
	declareNode.append_attribute("version") = "1.0";
	declareNode.append_attribute("encoding") = "UTF-8";

	xml_node element = xmlData.append_child("Object_Data");

	xml_node nodeCamera = element.append_child("Objects");
	nodeCamera.append_attribute("category") = "Camera";

	xml_node childCameraex = nodeCamera.append_child("EyePosX");
	childCameraex.append_child(node_pcdata).set_value(to_string(cameraData.POSITION.x).c_str());
	xml_node childCameraey = nodeCamera.append_child("EyePosY");
	childCameraey.append_child(node_pcdata).set_value(to_string(cameraData.POSITION.y).c_str());
	xml_node childCameraez = nodeCamera.append_child("EyePosZ");
	childCameraez.append_child(node_pcdata).set_value(to_string(cameraData.POSITION.z).c_str());

	xml_node childCamerarx = nodeCamera.append_child("EyeRotX");
	childCamerarx.append_child(node_pcdata).set_value(to_string(cameraData.ROTATION.x).c_str());
	xml_node childCamerary = nodeCamera.append_child("EyeRotY");
	childCamerary.append_child(node_pcdata).set_value(to_string(cameraData.ROTATION.y).c_str());
	xml_node childCamerarz = nodeCamera.append_child("EyeRotZ");
	childCamerarz.append_child(node_pcdata).set_value(to_string(cameraData.ROTATION.z).c_str());

	xml_node childCameratx = nodeCamera.append_child("TargetPosX");
	childCameratx.append_child(node_pcdata).set_value(to_string(cameraData.SCALE.x).c_str());
	xml_node childCameraty = nodeCamera.append_child("TargetPosY");
	childCameraty.append_child(node_pcdata).set_value(to_string(cameraData.SCALE.y).c_str());
	xml_node childCameratz = nodeCamera.append_child("TargetPosZ");
	childCameratz.append_child(node_pcdata).set_value(to_string(cameraData.SCALE.z).c_str());

	vector<sObjectData>::iterator iter;
	for (iter = vec.begin(); iter != vec.end(); ++iter)
	{
		xml_node nodeChild = element.append_child("Objects");
		xml_node child = nodeChild.append_child("ID");
		child.append_child(node_pcdata).set_value(iter->ID.c_str());
		xml_node childpx = nodeChild.append_child("PosX");
		childpx.append_child(node_pcdata).set_value(to_string(iter->POSITION.x).c_str());
		xml_node childpy = nodeChild.append_child("PosY");
		childpy.append_child(node_pcdata).set_value(to_string(iter->POSITION.y).c_str());
		xml_node childpz = nodeChild.append_child("PosZ");
		childpz.append_child(node_pcdata).set_value(to_string(iter->POSITION.z).c_str());

		xml_node childrx = nodeChild.append_child("RotX");
		childrx.append_child(node_pcdata).set_value(to_string(iter->ROTATION.x).c_str());
		xml_node childry = nodeChild.append_child("RotY");
		childry.append_child(node_pcdata).set_value(to_string(iter->ROTATION.y).c_str());
		xml_node childrz = nodeChild.append_child("RotZ");
		childrz.append_child(node_pcdata).set_value(to_string(iter->ROTATION.z).c_str());

		xml_node childsx = nodeChild.append_child("ScaleX");
		childsx.append_child(node_pcdata).set_value(to_string(iter->SCALE.x).c_str());
		xml_node childsy = nodeChild.append_child("ScaleY");
		childsy.append_child(node_pcdata).set_value(to_string(iter->SCALE.y).c_str());
		xml_node childsz = nodeChild.append_child("ScaleZ");
		childsz.append_child(node_pcdata).set_value(to_string(iter->SCALE.z).c_str());

		xml_node childsound = nodeChild.append_child("EffectSound");
		childsound.append_child(node_pcdata).set_value(iter->SOUNDTAG.c_str());
		xml_node childlock = nodeChild.append_child("Lock");
		childlock.append_child(node_pcdata).set_value(to_string(iter->LOCK).c_str());
	}

	stringstream ss;
	ss << path << m_xmlDataPath << fileName;
	_bool result = xmlData.save_file(ss.str().c_str());
}

// Load Lights from file
void CXMLParser::LoadLightData(string path, string fileName, vector<sLightData>& vec)
{
	stringstream ss;
	ss << path << m_xmlDataPath << fileName;

	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Light_Data").children();
	if (list.empty())
		return;

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_node_iterator childIter;

		sLightData data;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(childNode.name(), "Name")) data.NAME = childNode.child_value();

			if (!strcmp(childNode.name(), "PosX")) data.POSITION.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "PosY")) data.POSITION.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "PosZ")) data.POSITION.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "PosW")) data.POSITION.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "DirX")) data.DIRECTION.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DirY")) data.DIRECTION.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DirZ")) data.DIRECTION.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DirW")) data.DIRECTION.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "DiffuseX")) data.DIFFUSE.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DiffuseY")) data.DIFFUSE.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DiffuseZ")) data.DIFFUSE.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "DiffuseW")) data.DIFFUSE.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "SpecularX")) data.SPECULAR.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "SpecularY")) data.SPECULAR.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "SpecularZ")) data.SPECULAR.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "SpecularW")) data.SPECULAR.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "AmbientX")) data.AMBIENT.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AmbientY")) data.AMBIENT.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AmbientZ")) data.AMBIENT.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AmbientW")) data.AMBIENT.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "AttenX")) data.ATTEN.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AttenY")) data.ATTEN.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AttenZ")) data.ATTEN.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "AttenW")) data.ATTEN.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "Param1X")) data.PARAM1.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param1Y")) data.PARAM1.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param1Z")) data.PARAM1.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param1W")) data.PARAM1.w = (_float)atof(childNode.child_value());

			if (!strcmp(childNode.name(), "Param2X")) data.PARAM2.x = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param2Y")) data.PARAM2.y = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param2Z")) data.PARAM2.z = (_float)atof(childNode.child_value());
			if (!strcmp(childNode.name(), "Param2W")) data.PARAM2.w = (_float)atof(childNode.child_value());
		}
		vec.push_back(data);
	}
}

// Save Lights from file
void CXMLParser::SaveLightData(string path, string fileName, vector<sLightData>& vec)
{
	xml_document xmlData;

	xml_node declareNode = xmlData.append_child(node_declaration);
	declareNode.append_attribute("version") = "1.0";
	declareNode.append_attribute("encoding") = "UTF-8";

	xml_node element = xmlData.append_child("Light_Data");

	vector<sLightData>::iterator iter;
	for (iter = vec.begin(); iter != vec.end(); ++iter)
	{
		xml_node nodeChild = element.append_child("Lights");
		xml_node child = nodeChild.append_child("Name");
		child.append_child(node_pcdata).set_value(iter->NAME.c_str());
		xml_node childpx = nodeChild.append_child("PosX");
		childpx.append_child(node_pcdata).set_value(to_string(iter->POSITION.x).c_str());
		xml_node childpy = nodeChild.append_child("PosY");
		childpy.append_child(node_pcdata).set_value(to_string(iter->POSITION.y).c_str());
		xml_node childpz = nodeChild.append_child("PosZ");
		childpz.append_child(node_pcdata).set_value(to_string(iter->POSITION.z).c_str());
		xml_node childpw = nodeChild.append_child("PosW");
		childpw.append_child(node_pcdata).set_value(to_string(iter->POSITION.w).c_str());

		xml_node childdx = nodeChild.append_child("DirX");
		childdx.append_child(node_pcdata).set_value(to_string(iter->DIRECTION.x).c_str());
		xml_node childdy = nodeChild.append_child("DirY");
		childdy.append_child(node_pcdata).set_value(to_string(iter->DIRECTION.y).c_str());
		xml_node childdz = nodeChild.append_child("DirZ");
		childdz.append_child(node_pcdata).set_value(to_string(iter->DIRECTION.z).c_str());
		xml_node childdw = nodeChild.append_child("DirW");
		childdw.append_child(node_pcdata).set_value(to_string(iter->DIRECTION.w).c_str());

		xml_node childdfx = nodeChild.append_child("DiffuseX");
		childdfx.append_child(node_pcdata).set_value(to_string(iter->DIFFUSE.x).c_str());
		xml_node childdfy = nodeChild.append_child("DiffuseY");
		childdfy.append_child(node_pcdata).set_value(to_string(iter->DIFFUSE.y).c_str());
		xml_node childdfz = nodeChild.append_child("DiffuseZ");
		childdfz.append_child(node_pcdata).set_value(to_string(iter->DIFFUSE.z).c_str());
		xml_node childdfw = nodeChild.append_child("DiffuseW");
		childdfw.append_child(node_pcdata).set_value(to_string(iter->DIFFUSE.w).c_str());

		xml_node childsx = nodeChild.append_child("SpecularX");
		childsx.append_child(node_pcdata).set_value(to_string(iter->SPECULAR.x).c_str());
		xml_node childsy = nodeChild.append_child("SpecularY");
		childsy.append_child(node_pcdata).set_value(to_string(iter->SPECULAR.y).c_str());
		xml_node childsz = nodeChild.append_child("SpecularZ");
		childsz.append_child(node_pcdata).set_value(to_string(iter->SPECULAR.z).c_str());
		xml_node childsw = nodeChild.append_child("SpecularW");
		childsw.append_child(node_pcdata).set_value(to_string(iter->SPECULAR.w).c_str());

		xml_node childax = nodeChild.append_child("AmbientX");
		childax.append_child(node_pcdata).set_value(to_string(iter->AMBIENT.x).c_str());
		xml_node childay = nodeChild.append_child("AmbientY");
		childay.append_child(node_pcdata).set_value(to_string(iter->AMBIENT.y).c_str());
		xml_node childaz = nodeChild.append_child("AmbientZ");
		childaz.append_child(node_pcdata).set_value(to_string(iter->AMBIENT.z).c_str());
		xml_node childaw = nodeChild.append_child("AmbientW");
		childaw.append_child(node_pcdata).set_value(to_string(iter->AMBIENT.w).c_str());

		xml_node childatx = nodeChild.append_child("AttenX");
		childatx.append_child(node_pcdata).set_value(to_string(iter->ATTEN.x).c_str());
		xml_node childaty = nodeChild.append_child("AttenY");
		childaty.append_child(node_pcdata).set_value(to_string(iter->ATTEN.y).c_str());
		xml_node childatz = nodeChild.append_child("AttenZ");
		childatz.append_child(node_pcdata).set_value(to_string(iter->ATTEN.z).c_str());
		xml_node childatw = nodeChild.append_child("AttenW");
		childatw.append_child(node_pcdata).set_value(to_string(iter->ATTEN.w).c_str());

		xml_node childp1x = nodeChild.append_child("Param1X");
		childp1x.append_child(node_pcdata).set_value(to_string(iter->PARAM1.x).c_str());
		xml_node childp1y = nodeChild.append_child("Param1Y");
		childp1y.append_child(node_pcdata).set_value(to_string(iter->PARAM1.y).c_str());
		xml_node childp1z = nodeChild.append_child("Param1Z");
		childp1z.append_child(node_pcdata).set_value(to_string(iter->PARAM1.z).c_str());
		xml_node childp1w = nodeChild.append_child("Param1W");
		childp1w.append_child(node_pcdata).set_value(to_string(iter->PARAM1.w).c_str());

		xml_node childp2x = nodeChild.append_child("Param2X");
		childp2x.append_child(node_pcdata).set_value(to_string(iter->PARAM2.x).c_str());
		xml_node childp2y = nodeChild.append_child("Param2Y");
		childp2y.append_child(node_pcdata).set_value(to_string(iter->PARAM2.y).c_str());
		xml_node childp2z = nodeChild.append_child("Param2Z");
		childp2z.append_child(node_pcdata).set_value(to_string(iter->PARAM2.z).c_str());
		xml_node childp2w = nodeChild.append_child("Param2W");
		childp2w.append_child(node_pcdata).set_value(to_string(iter->PARAM2.w).c_str());
	}

	stringstream ss;
	ss << path << m_xmlDataPath << fileName;
	_bool result = xmlData.save_file(ss.str().c_str());
}

// Load Language Data from file
void CXMLParser::LoadLanguageData(string path, string fileName, unordered_map<string, string>& map)
{
	stringstream ss;
	ss << path << m_xmlDataPath << "lang\\" << fileName;

	xml_document xmlData;
	xml_parse_result result = xmlData.load_file(ss.str().c_str());
	if (!result)
		return;

	xml_object_range<xml_node_iterator> list = xmlData.child("Language_Data").children();
	if (list.empty())
		return;

	xml_node_iterator iter;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		xml_node node = *iter;
		xml_node_iterator childIter;

		string key;
		string content;
		for (childIter = node.children().begin(); childIter != node.children().end(); ++childIter)
		{
			xml_node childNode = *childIter;
			if (!strcmp(childNode.name(), "Key")) key = childNode.child_value();
			if (!strcmp(childNode.name(), "Content")) content = childNode.child_value();
		}
		map.insert(unordered_map<string, string>::value_type(key, content));
	}
}
