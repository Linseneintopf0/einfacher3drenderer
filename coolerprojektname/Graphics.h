#pragma once
#include "windowsexcept.h"
#include <d3d11.h>
#include "Color.h"
#include "Window.h"
#include <wrl.h>
#include "Camera.h"
#include <array>

class Graphics
{
public:
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
	struct Index
	{
		struct
		{
			const unsigned short a;
			const unsigned short b;
			const unsigned short c;
		} corners;
	};

public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void SetupVertexAndIndexBuffer(Graphics::Index* indices, Graphics::Vertex* vertices, unsigned short trianglecount, unsigned short vertexcount);
	void SetupConstantBufferAndShaders();
	void SetupOutputmerger(unsigned short width, unsigned short height);
	void UpdateAspectratio(unsigned short width, unsigned short height);
	void UpdateConstantBuffer(unsigned short width, unsigned short height, Camera::transformstruct transformstruct);

	void Draw();
	void ClearBuffer(float r, float g, float b) noexcept;
	void ClearBuffer(const Color* color) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};