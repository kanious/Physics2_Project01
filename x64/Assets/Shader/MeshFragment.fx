#version 460

in vec4 fColour;
in vec4 fNormal;
in vec4 fVtxWorldPos;
in vec4 fTexUV;
in vec4 fTangent;
in vec4 fBinormal;
in mat4 fInvMat;

uniform sampler2D diffTexture;
uniform sampler2D normalTexture;

uniform bool isLightEnable;
uniform bool isSelected;
uniform bool isTransparency;

struct sLight
{
	vec4       position;
	vec4       direction;
	vec4       diffuse;
	vec4       specular;		// rgp = highlight color, w = power
	vec4       ambient;			// rgp = ambient color, w = power
	vec4       atten;			// x = constant, y = linear, z = quadratic, w = cutoff range
	vec4       param1;			// x = lightType, y = inner angle, z = outer angle, w = TBD(?)
	vec4       param2;			// x = turn (1:on, 0:off)
};
const int DIRECTIONAL_LIGHT_TYPE = 0;
const int POINT_LIGHT_TYPE = 1;
const int SPOT_LIGHT_TYPE = 2;
const int NUMBEROFLIGHTS = 20;
uniform sLight theLights[NUMBEROFLIGHTS];
uniform vec4 eyeLocation;

vec4 calculateLight(vec3 diffuseColor, vec3 normal, vec3 vtxWorldPos);

out vec4 daColor;
void main()
{
	vec4 diffColour = texture(diffTexture, fTexUV.xy);

	if (!isLightEnable)
	{
		if (!isSelected)
			daColor = vec4(diffColour.xyz, 1.0f);
		else
			daColor = vec4(diffColour.x + 0.5f, diffColour.y, diffColour.z + 0.5f, 1.0f);
		return;
	}

	vec3 vDiffuse = vec3(diffColour.x, diffColour.y, diffColour.z);

	vec3 normal = vec3(fNormal.x, fNormal.y, fNormal.z);
	//vec3 normal = texture(normalTexture, fTexUV.xy).rgb;

	vec3 vtxWorldPos = vec3(fVtxWorldPos.x, fVtxWorldPos.y, fVtxWorldPos.z);
	vec4 outColour = calculateLight(vDiffuse, normal.xyz, vtxWorldPos);

	//float alpha = 1.f;

	//if (diffColour.x + diffColour.y + diffColour.z == 3.f)
	//	alpha = 0.f;

	if (isSelected)
		daColor = vec4(outColour.x + 0.3f, outColour.y, outColour.z + 0.3f, diffColour.a);
	else
		daColor = vec4(outColour.x, outColour.y, outColour.z, diffColour.a);
	
	if (isTransparency)
		daColor.a = diffColour.x * 2.f;

	//diffColour = theLights[0].diffuse;
	//daColor = vec4(diffColour.x, diffColour.y, diffColour.z, 1.0f);
	//daColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

vec4 calculateLight(vec3 diffuseColor, vec3 normal, vec3 vtxWorldPos)
{
	vec4 finalColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec3 norm = normalize(normal);
	for (int i = 0; i < NUMBEROFLIGHTS; ++i)
	{
		if (0.0f == theLights[i].param2.x)
			continue;

		int lightType = int(theLights[i].param1.x);

		//DIRECTIONAL_LIGHT_TYPE
		if (lightType == DIRECTIONAL_LIGHT_TYPE)
		{
			vec3 lightContrib = theLights[i].diffuse.rgb;
			vec3 directionalDir = theLights[i].direction.xyz;
			directionalDir = normalize(directionalDir);
			float dot = dot(-directionalDir, norm);
			dot = max(0.0f, dot);
			float ambientPower = theLights[i].diffuse.w * 0.1f;
			lightContrib *= (dot + ambientPower);

			float lightPower = theLights[i].diffuse.w;
			vec3 result = diffuseColor * theLights[i].diffuse.xyz * lightContrib * lightPower;
			finalColour.xyz += result;

			continue;
		}

		//POINT_LIGHT_TYPE
		vec3 vLightToVertex = theLights[i].position.xyz - vtxWorldPos;
		float distanceToLight = length(vLightToVertex);
		//if (theLights[i].atten.w < distanceToLight)
		//	continue;

		vec3 lightDir = normalize(vLightToVertex);
		float dotPoint = dot(lightDir, normal);
		dotPoint = max(0.0f, dotPoint);
		float lightPower = theLights[i].diffuse.w;
		vec3 lightDiffuseContrib = dotPoint * theLights[i].diffuse.rgb * lightPower;
		
		//specular
		vec3 lightSpecularContrib = vec3(0.0f);
		vec3 vReflect = reflect(-lightDir, norm);
		vec3 vEye = normalize(eyeLocation.xyz - vtxWorldPos);
		float specularPower = theLights[i].specular.w;
		lightSpecularContrib =
			pow(max(0.0f, dot(vEye, vReflect)), specularPower) *
			theLights[i].specular.rgb;

		float attenuation = 1.0f /
			(theLights[i].atten.x +
			 theLights[i].atten.y * distanceToLight +
			 theLights[i].atten.z * distanceToLight * distanceToLight);

		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;

		if (lightType == SPOT_LIGHT_TYPE)
		{
			vec3 vtxToLight = vtxWorldPos - theLights[i].position.xyz;
			vtxToLight = normalize(vtxToLight);
			float currentRayAngle =
				dot(vtxToLight, theLights[i].direction.xyz);
			currentRayAngle = max(0.0f, currentRayAngle);

			float phi = cos(radians(theLights[i].param1.z));
			float theta = cos(radians(theLights[i].param1.y));

			// outside of outerCone
			if (currentRayAngle < phi)
			{
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			// between innerCone and outerCone
			else if (currentRayAngle < theta)
			{
				//float penumbraRatio = (currentRayAngle - phi) /
				//	(theta - phi);

				float epsilon = theta - phi;
				float penumbraRatio = clamp((currentRayAngle - phi) / epsilon, 0.0, 1.0);

				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}

		}
		
		vec3 result = (diffuseColor * lightDiffuseContrib) + (theLights[i].specular.xyz * lightSpecularContrib);
		finalColour.xyz += result;
	}

	finalColour.r = min(1.0f, finalColour.r);
	finalColour.g = min(1.0f, finalColour.g);
	finalColour.b = min(1.0f, finalColour.b);
	finalColour.a = 1.0f;

	return finalColour;
}	