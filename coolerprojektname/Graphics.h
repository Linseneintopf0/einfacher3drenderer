#pragma once
#include "windowsexcept.h"
#include <d3d11.h>
#include "Color.h"
#include "Ausnahmen.h"
#include "Window.h"
#include <wrl.h>
#include "Camera.h"

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics & operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float color[] = { r,g,b,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}
	void ClearBuffer(const Color* color) noexcept
	{
		pContext->ClearRenderTargetView(pTarget.Get(), color->color);
	}
	void DrawTestTriangle(unsigned int width, unsigned int height, Camera::transformstruct transformstruct);

public:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

#pragma region AusnahmeD3D

public:class D3DAusnahme : public Ausnahmen
	{
	public:
		D3DAusnahme(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

};

#pragma endregion