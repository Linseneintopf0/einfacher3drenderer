#pragma once
#include "window.h"

class gfxupdate
{
public:
	class GfxAusnahme : public Ausnahmen
	{
	public:
		GfxAusnahme(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
public:
	static void DoFrame(Window &wnd);
};

#define GFXAUSNAHME(hr) gfxupdate::GfxAusnahme(__LINE__,__FILE__,hr)
