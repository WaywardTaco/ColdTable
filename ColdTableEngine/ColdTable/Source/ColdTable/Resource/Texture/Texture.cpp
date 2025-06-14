#include <stdexcept>
#include <ColdTable/Resource/Texture/Texture.h>

#include "ColdTable/Graphics/GraphicsDevice.h"

ColdTable::Texture::Texture(GraphicsDevicePtr sourceDevice, const wchar_t* fullpath) : Resource(fullpath)
{
	DirectX::ScratchImage imageData;
	HRESULT hr = DirectX::LoadFromWICFile(fullpath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);
	ColdTableGraphicsLogThrowOnFail(hr, "Failed to load image file")

	hr = DirectX::CreateTexture(&(*(sourceDevice->_d3dDevice).Get()), imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &_texture);

	ColdTableGraphicsLogThrowOnFail(hr, "Failed to load image data")

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc{};
	resViewDesc.Format = imageData.GetMetadata().format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
	resViewDesc.Texture2D.MostDetailedMip = 0;
	sourceDevice->_d3dDevice->CreateShaderResourceView(_texture, &resViewDesc, &_resourceView);
}

ColdTable::Texture::~Texture()
{
	_texture->Release();
}
