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
	Graphics(HWND hWnd, unsigned int height, unsigned int width);
	Graphics(const Graphics&) = delete;
	Graphics & operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	void ClearBuffer(const Color* color) noexcept;
	void DrawTestTriangle(unsigned int width, unsigned int height, Camera::transformstruct transformstruct);

public:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

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