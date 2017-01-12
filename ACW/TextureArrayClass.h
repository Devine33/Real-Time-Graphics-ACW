#pragma once
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&) = default;
	TextureArrayClass& operator=(const TextureArrayClass&) = default;
	~TextureArrayClass();
	bool Initialize(ID3D11Device*const device, const WCHAR*const filename1, const WCHAR*const filename2);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[2];

};

