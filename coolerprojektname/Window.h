#pragma once
#include "windowsexcept.h"
#include "Graphics.h"
#include <string>
#include <memory>
#include <vector>
#include "Color.h"
#include "Camera.h"

//Klassen Vordefinierungen für die Pointer zu Objekten dieser Klassen
class Camera;
class Scene;
class Graphics;

class Window
{
public:
	struct GraphicsStructure;
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
	Window(unsigned short Width, unsigned short Height, const char* Name, const GraphicsStructure &sGraphics) noexcept;		//constructor (duh)
	~Window();																										//destructer (duh duh)
	Window(const Window&) = delete;																					//verhindert kopieren von Objekten der Klasse "Window", durch 'Window kopie(original);'
	Window& operator=(const Window&) = delete;																		//verhindert kopieren von Objekten der Klasse "Window", durch 'Window kopie = original;'
	Graphics& pGraphicsGet();
	Scene& pSceneGet();
	Camera& pCameraGet();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgAPICon(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam, Window& pWindow) noexcept;
private:
	HWND hWindow;
	std::unique_ptr<Graphics> pGraphics;
	std::unique_ptr<Scene> pScene;
	std::unique_ptr<Camera> pCamera;
public:
	unsigned int HeldKeys = 0;
	unsigned int Width;
	unsigned int Height;
	static unsigned long WindowCount;									//Number of current Windows (to dynamically add and close them, and stop the program when this is 0
	static std::vector<Window*> WindowList;								//Vector with pointers to every Window Instance (for looping over in Graphical uses)

	struct GraphicsStructure {
		mutable std::string FilePath;
		std::unique_ptr<Color> ColorObj;
		int scale = 0;
	};
	const GraphicsStructure* structureGraphics;
};