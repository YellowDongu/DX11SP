#pragma once


enum class windowType
{
	Windowed,
	Full
};

enum class ShaderVar
{
	World,
	//View,
	//Projection,
	ViewProjection,
	BoneMatrix,

};

#define MaterialBuffer L"MaterialBufferInput"
#define MaterialBufferA "MaterialBufferInput"
#define DiffuseTextureBuffer L"diffuseTexture"
#define DiffuseTextureBufferA "diffuseTexture"
#define SpecularTextureBuffer l"specularTexture"
#define SpecularTextureBufferA "specularTexture"
#define NormalTextureBuffer l"normalTexture"
#define NormalTextureBufferA "normalTexture"