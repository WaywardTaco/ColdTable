#include <ColdTable/Graphics/GraphicsEngine.h>
#include <ColdTable/Graphics/GraphicsDevice.h>
#include <ColdTable/Graphics/DeviceContext.h>
#include <ColdTable/Graphics/VertexBuffer.h>
#include <d3dcompiler.h>

#include "ColdTable/Math/Vertex.h"

ColdTable::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{desc.base});

	auto& device = *_graphicsDevice;
	_deviceContext = device.CreateDeviceContext();
}

ColdTable::GraphicsEngine::~GraphicsEngine()
{
}

void ColdTable::GraphicsEngine::RegisterRenderable(RenderablePtr renderable)
{
	_renderables.push_back(renderable);
}

void ColdTable::GraphicsEngine::UnregisterRenderable(RenderablePtr renderable)
{
	std::vector<RenderablePtr>::iterator index{};
	for (auto itr = _renderables.begin(); itr != _renderables.end(); ++itr)
	{
		if (*itr == renderable)
		{
			index = itr;
			break;
		}
	}

	if (index != _renderables.end())
		_renderables.erase(index);
}

ColdTable::GraphicsDevicePtr ColdTable::GraphicsEngine::GetGraphicsDevice() noexcept
{
	return _graphicsDevice;
}


void ColdTable::GraphicsEngine::Render(SwapChain& swapChain, ConstantBufferPtr constantBuffer, Rect viewportSize)
{
	auto& context = *_deviceContext;
	context.ClearAndSetBackBuffer(swapChain, {0.2, 0.2, 0.5, 1});

	context.SetViewportSize(viewportSize);

	ConstantBufferContent constant;
	constant.m_time = ::GetTickCount();

	constantBuffer->Update(&context, &constant);
	context.BindConstantBuffer(constantBuffer);

	for (auto renderable : _renderables)
	{
		context.Draw(renderable);
	}

	auto& device = *_graphicsDevice;
	device.ExecuteCommandList(context);

	swapChain.Present();
}

void ColdTable::GraphicsEngine::SetViewportSize(Rect size)
{
	_deviceContext->SetViewportSize(size);
}

ColdTable::VertexBufferPtr ColdTable::GraphicsEngine::CreateVertexBuffer()
{
	return _graphicsDevice->CreateVertexBuffer();
}

ColdTable::ConstantBufferPtr ColdTable::GraphicsEngine::CreateConstantBuffer()
{
	return _graphicsDevice->CreateConstantBuffer();
}

ColdTable::ShaderPtr ColdTable::GraphicsEngine::CreateShader(const wchar_t* vertexShaderSrc, const wchar_t* pixelShaderSrc)
{
	return _graphicsDevice->CreateShader(vertexShaderSrc, pixelShaderSrc);
}

void ColdTable::GraphicsEngine::UseShader(const ShaderPtr& shader)
{
	//_deviceContext->UseShader(shader);
}
