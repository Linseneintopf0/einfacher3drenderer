#include "Graphics.h"
#include <sstream>
#include "windowsexcept.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
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
	HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(
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

	if (FAILED(hr)) { throw Graphics::D3DAusnahme(__LINE__, __FILE__, hr); } //throw wenn Fehler

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	if (FAILED(hr)) { throw Graphics::D3DAusnahme(__LINE__, __FILE__, hr); } //throw wenn Fehler

	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);

	if (FAILED(hr)) { throw Graphics::D3DAusnahme(__LINE__, __FILE__, hr); } //throw wenn Fehler

	pBackBuffer->Release();
}

void Graphics::DrawTestTriangle(unsigned int width, unsigned int height, float angle)
{
	float aspectratio = (float)height / width;

#pragma region IloveIloveIloveBuffer

	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color ;
	};

	Vertex verticies[] =
	{//    x      y     r    g    b    a (wird eh überschrieben, allerdings braucht die PrimitiveTopology den Alpha Wert)
		{-0.2f,  0.0f, 255,   0,   0,   0},
		{ 0.2f,  0.0f,   0,   0, 255,   0},
		{ 0.0f,  1.0f, 255, 255, 255,   0},
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;		//ein ComPointer für den Buffer erstellen (ComPointer releasen nach abschluss der COM Interface verwendung -> kein Memory Leck)

	D3D11_BUFFER_DESC bd = {};								//Buffereinstellungen für D3D
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = sizeof(verticies);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};							//den "verticies" array als Resource erklären
	sd.pSysMem = verticies;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);		//den Buffer auf dem Device erstellen

	const unsigned short indices[] =
	{
		0, 2, 1,
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);


	struct ConstantBuffer
	{													//Const Buffer = Buffer der sich wärend eines Frames nicht ändert
		struct											//const buffer != const in c++
		{
			float element[16];						//erstellen einer übersichtlichen 4x4 Matrix
		} transformation;
	};
	const ConstantBuffer cb =
	{
		
		aspectratio * std::cos(angle), -std::sin(angle), 0.0f, 0.0f,		//Mathematische Rotationsmatrix fürs Drehen um die Z Achse (Die in den Bildschirm hinein)
		aspectratio * std::sin(angle),  std::cos(angle), 0.0f, 0.0f,		//als Const definiert, da sie sich nur zwischen Frames ändert (hier im Code sowieso jeden Frame neuerstellt)
		           0.0f,             0.0f,  1.0f, 0.0f,
		           0.0f,             0.0f,  0.0f, 1.0f,
		
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset); //Erstellen von dem Buffern

#pragma endregion

#pragma region Shader Erstellen


	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;					//Blob erstellen -> spezieller pointer für das laden von Shadern (Pixel und Vertex), wird nach dem Erstellen released

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);	//PixelShader erstellen

	pContext->PSSetShader(pPixelShader.Get(), 0, 0);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso",&pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);//VertexShader erstellen

	pContext->VSSetShader(pVertexShader.Get(), 0, 0);

#pragma endregion

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0, 0);
}

void Graphics::EndFrame()
{
	pSwap->Present(1, 0);
}

#pragma region AusnahmeRegelung

Graphics::D3DAusnahme::D3DAusnahme(int line, const char* file, HRESULT hr) noexcept
	:
	Ausnahmen(line, file),
	hr(hr)
{
}

const char* Graphics::D3DAusnahme::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: " << GetErrorCode() << std::endl
		<< "Description: " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::D3DAusnahme::GetType() const noexcept
{
	return "Fenster Ausnahme";
}

std::string Graphics::D3DAusnahme::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pErrorString = nullptr;
	DWORD diStringLength = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pErrorString),
		0, nullptr);
	if (diStringLength == 0)
	{
		return "Unbekannter Code";
	}
	std::string errorString = pErrorString;
	LocalFree(pErrorString);
	return errorString;
}

HRESULT Graphics::D3DAusnahme::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::D3DAusnahme::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

#pragma endregion
