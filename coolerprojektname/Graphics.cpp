#include "Graphics.h"
#include <sstream>
#include "windowsexcept.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace dx = DirectX; //DirectX Math Library für Mathematische Operationen

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
#pragma region SwapChainAndDevice

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D_FEATURE_LEVEL FL = {D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL* pFL = &FL;
		D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		pFL,
		&pContext
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);

#pragma endregion
}

void Graphics::SetupVertexAndIndexBuffer(Graphics::Index* indices, Graphics::Vertex* vertices, unsigned short trianglecount, unsigned short vertexcount)
{
#pragma region VertexBuffer

	D3D11_BUFFER_DESC vbd = {};								//Buffereinstellungen für D3D
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.ByteWidth = vertexcount * sizeof(Graphics::Vertex);
	vbd.StructureByteStride = sizeof(Graphics::Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};							//den "verticies" array als Resource erklären
	sd.pSysMem = vertices;

	pDevice->CreateBuffer(&vbd, &sd, &pVertexBuffer);		//den Buffer auf dem Device erstellen
	const UINT stride = sizeof(Graphics::Vertex);
	const UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

#pragma endregion

#pragma region IndexBuffer

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = trianglecount * sizeof(Graphics::Index);
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

#pragma endregion

#pragma region PrimitiveTopology

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#pragma endregion
}

void Graphics::SetupConstantBufferAndShaders()
{
#pragma region ConstantBuffer

	struct ConstantBuffer
	{													//Const Buffer = Buffer der sich wärend eines Frames nicht ändert
		dx::XMMATRIX transform;							//Mathematische Rotationsmatrix fürs Drehen um die Z Achse (Die in den Bildschirm hinein)
	};

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.StructureByteStride = 0;
	
	pDevice->CreateBuffer(&cbd, nullptr, &pConstantBuffer);

	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

#pragma endregion

#pragma region Shaders

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);	//PixelShader erstellen

	pContext->PSSetShader(pPixelShader.Get(), 0, 0);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);//VertexShader erstellen

	pContext->VSSetShader(pVertexShader.Get(), 0, 0);

#pragma endregion

#pragma region InputLayout

	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	pContext->IASetInputLayout(pInputLayout.Get());
#pragma endregion
}

void Graphics::SetupOutputmerger(unsigned short width, unsigned short height)
{
#pragma region DepthStencil

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;

	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	pContext->OMSetDepthStencilState(pDSState.Get(), 1);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthStencilView);

	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDepthStencilView.Get());
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

#pragma endregion

#pragma region ViewPort

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

#pragma endregion
}

void Graphics::UpdateAspectratio(unsigned short width, unsigned short height)
{
	//Render Target entfernen
	pContext->OMSetRenderTargets(0, 0, 0);
	//Render Target Pointer leeren
	pTarget = nullptr;
	//Swap-Chain Buffer anpassen
	pSwap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	//Alte Swap-Chain-Buffer einstellungen kriegen
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

	//Render Target wieder deklarieren
	pDevice->CreateRenderTargetView(pBuffer.Get(), NULL, pTarget.GetAddressOf());

	//Depth Stencil und Viewport wieder anpassen
	Graphics::SetupOutputmerger(width, height);
}

void Graphics::UpdateConstantBuffer(unsigned short width, unsigned short height, Camera::transformstruct transformstruct)
{
#pragma region UpdateConstantBuffer

	D3D11_MAPPED_SUBRESOURCE cbsubresource; //subresource erklären
	float aspectratio = (float)height / width; //Seitenverhältnis für Projektionsrechnung


	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixTranslation(-0.5f, -0.5f, -0.5f) *
				dx::XMMatrixRotationX(-transformstruct.rx) *
				dx::XMMatrixRotationY(-transformstruct.ry) *
				dx::XMMatrixRotationZ(-transformstruct.rz) *
				dx::XMMatrixTranslation(transformstruct.x, transformstruct.y, transformstruct.z) *
				dx::XMMatrixTranslation(0.0f, 0.0f, 8.0f) *
				dx::XMMatrixPerspectiveLH(1.0f, aspectratio, 1.0f, 20.0f)
			)
		}
	};

	pContext->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &cbsubresource);
	memcpy(cbsubresource.pData, &cb, sizeof(cb));
	pContext->Unmap(pConstantBuffer.Get(), 0);

#pragma endregion
}

void Graphics::Draw()
{
	D3D11_BUFFER_DESC ibd;
	pIndexBuffer->GetDesc(&ibd);
	pContext->DrawIndexed(3*(ibd.ByteWidth/sizeof(unsigned short)), 0, 0);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::ClearBuffer(const Color* color) noexcept
{
	Graphics::ClearBuffer(color->color[0], color->color[1], color->color[2]);
}

void Graphics::EndFrame()
{
	pSwap->Present(1, 0);
}