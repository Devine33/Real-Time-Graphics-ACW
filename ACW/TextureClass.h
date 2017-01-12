#pragma once
#include <d3d11.h>
#include <wrl/client.h>
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*const dev, const WCHAR*const filename);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() const;

private:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//ID3D11ShaderResourceView* m_texture;

};

