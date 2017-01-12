#include "TextureClass.h"
#include "../include/DDSTextureLoader.h"

TextureClass::TextureClass(): m_texture(nullptr)
{
}

TextureClass::TextureClass(const TextureClass&)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device*const dev, const WCHAR*const filename)
{
	HRESULT result;
	
	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(dev, filename, nullptr, m_texture.GetAddressOf(),0,nullptr);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TextureClass::GetTexture() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	 return m_texture.Get();
}
