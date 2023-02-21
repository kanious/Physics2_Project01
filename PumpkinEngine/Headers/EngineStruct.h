#ifndef _ENGINESTRUCT_H_
#define _ENGINESTRUCT_H_

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	typedef struct sVertex_RGBA_XYZ_N_UV_T_BiN_Bones
	{
		glm::vec4 vColour;
		glm::vec4 vPos;
		glm::vec4 vNormal;
		glm::vec4 vTexUV;
		glm::vec4 vTangent;
		glm::vec4 vBinormal;
		glm::vec4 vBoneID;
		glm::vec4 vBoneWeight;
	}VTX;

	typedef struct sIndex
	{
		unsigned int _0;
		unsigned int _1;
		unsigned int _2;
	}IDX;

	typedef struct sTriangle
	{
		glm::vec3 p0;
		glm::vec3 p1;
		glm::vec3 p2;
	}TRIANGLE;

	enum eModelType
	{
		xyz_index,
		xyz_normal_index,
		xyz_normal_texUV_index,
		xyz_normal_texUV_index_texNum,
		xyz_normal_color_index
	};

	struct sUserInfo
	{
		int64_t userId;
		std::string email;
		std::string username;
		std::string lastlogin;
		std::string creationdate;
	};

	enum MessageType
	{
		Msg_UserInput,
		Msg_GameState
	};
}

#endif //_ENGINESTRUCT_H_