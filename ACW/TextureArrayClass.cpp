//referenced from rastertek to 
#include "TextureArrayClass.h"
#include "../include/DDSTextureLoader.h"


TextureArrayClass::TextureArrayClass() : m_textures{nullptr,nullptr}
{
}


TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device*const device, const WCHAR*const filename1, const WCHAR*const filename2)
{
	HRESULT result;

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, filename1, nullptr, &m_textures[0], 0, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	// Load the second texture in.
	result = DirectX::CreateDDSTextureFromFile(device, filename2, nullptr, &m_textures[1], 0, nullptr);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}
ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray() // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_textures;
}

