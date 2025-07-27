#pragma once
#include "windowsexcept.h"
#include "Ausnahmen.h"
#include "Graphics.h"
#include <memory>
#include <vector>
#include "Color.h"

class Graphics;
class Window
{
public:

	//Jedes Fenster hat eine graphicstuct, die hier declariert ist
	struct graphicsstruct {
		mutable std::string dateipfad;
		const Color* colorobj;
	};
	const graphicsstruct* sGfx;
public:
	class WindowAusnahme : public Ausnahmen
	{
	public:
		WindowAusnahme(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;						//deklariert Funktion GetName() als public
		static HINSTANCE GetInstance() noexcept;						//deklariert Funktion GetInstance() als public
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;						//verhindert kopieren von Objekten der Klasse "WindowClass", durch 'WindowClass kopie(original);'
		WindowClass& operator=(const WindowClass&) = delete;			//verhindert kopieren von Objekten der Klasse "WindowClass", durch 'Windowclass kopie = original;'
		static constexpr const char* wndClassName = "ersteklasse";		//Name der WindowClass
		static WindowClass wndClass;									//erzeugt Objekt "wndClass"
		HINSTANCE hInst;												//erzeugt variable "hInst" als HINSTANCE
	};
public:
	Window(int width, int height, const char* name, const graphicsstruct &sGfx) noexcept;		//constructor (duh)
	~Window();																				//destructer (duh duh)
	Window(const Window&) = delete;															//verhindert kopieren von Objekten der Klasse "Window", durch 'Window kopie(original);'
	Window& operator=(const Window&) = delete;												//verhindert kopieren von Objekten der Klasse "Window", durch 'Window kopie = original;'
	Graphics& pGraphics();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgAPICon(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	mutable std::string title;
public:
	float angle = 0;
	WPARAM currentpressedkey = 0;
	unsigned int width;
	unsigned int height;
	static unsigned long WindowCount;									//Number of current Windows (to dynamically add and close them, and stop the program when this is 0
	static std::vector<Window*> windowlist;								//Vector with pointers to every Window Instance (for looping over in Graphical uses)
};

#define WINDOWAUSNAHME(hr) Window::WindowAusnahme(__LINE__,__FILE__,hr)