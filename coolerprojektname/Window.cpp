#include "Window.h"
#include <string>
#include <sstream>
#include "resource.h"
#include "Camera.h"
#include "Scene.h"

//Definition des statischen Window::WindowClass Objekts
Window::WindowClass Window::WindowClass::wndClass;

//Definition der Statischen Variablen
unsigned long Window::WindowCount = 0;
std::vector<Window*> Window::WindowList = {};

#pragma region WindowClass Shinanigans

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	//Window Class Einstellungen
	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImageW(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0));
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImageW(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0));
	RegisterClassExA(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClassA(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

#pragma endregion

#pragma region Window Shenanigans

Window::Window(unsigned short Width, unsigned short Height, const char* Name, const GraphicsStructure& structureGraphics) noexcept
	:
	Height(Height),
	Width(Width),
	structureGraphics(&structureGraphics)
{
	RECT wr;
	int maximize = NULL; //Maximieren des Fensters, wenn Fensterdimension außerhalb der gültigen Dimensionen ist

	//Erstellen eines Rechtecks, der Größe des Fensters
	wr.left = 0;
	wr.top = 0;
	if	((Height == 0 && Width == 0) || 
		(Height > GetSystemMetrics(SM_CYFULLSCREEN) && Width > GetSystemMetrics(SM_CXFULLSCREEN))) {
		Window::Height = GetSystemMetrics(SM_CYFULLSCREEN)-100;
		Window::Width = GetSystemMetrics(SM_CXFULLSCREEN)-500;
		maximize = WS_MAXIMIZE;
	}
	wr.right = Window::Width + wr.left;
	wr.bottom = Window::Height + wr.top;

	//Adjustieren des Rechtecks an die vorhandene Menüleiste
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX| WS_SYSMENU, FALSE);

	//Fenster erstellen
	Window::hWindow = CreateWindowExA(0,
		WindowClass::GetName(), Name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE | maximize,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, WindowClass::GetInstance(), this
	);

	if (maximize) {
		GetWindowRect(hWindow, &wr);
		Window::Height = wr.bottom - wr.top;
		Window::Width = wr.right - wr.left;
	}

	//Fenstertitel korrigieren (keine Ahnung warum der beim Erstellen nicht richtig ist)
	SetWindowTextA(hWindow, (char*)&Name[0]);

	//Erstellen des Graphics Objektes
	pGraphics = std::make_unique<Graphics>(hWindow);
	pScene = std::make_unique<Scene>(*this);
	pCamera = std::make_unique<Camera>();

	//Window Pointer Management
	WindowList.push_back(this);
	WindowCount++;
}

Window::~Window()
{
	auto objname = std::find(WindowList.begin(), WindowList.end(), this);
	WindowList.erase(objname);
	DestroyWindow(hWindow);
	WindowCount--;
}

#pragma endregion

#pragma region Pointer zu unterliegenden Objekten kriegen

Graphics& Window::pGraphicsGet()  
{  
    return *pGraphics;
}

Scene& Window::pSceneGet()
{
	return *pScene;
}

Camera& Window::pCameraGet()
{
	return *pCamera;
}

#pragma endregion

#pragma region MessageSetup 

//Erklärung der folgenden Funktionen
///Windows API kann keine Nicht-Statischen Funktionen aufrufen (hat keine Ahnung von C++, kennt nur das "Programm") -> WinAPIPointer für Nicht-Statische Funktion und
/// Fenster erstellen (Kann mit pointern zu Funktionen arbeiten)
///Die erste Funktion erzeugt zwei Pointer auf der WindowsAPI, die auf das Fenster und die HandleMsgAPICon funktion zeigen.
///Die zweite Funtion ruft die HandleMsg Funktion via pointer auf -> WinAPI gicht kein Anfall.
///Nur weil beide Funktionen "WINAPI" als calling convention haben und static sind, sind sie per WinAPI aufruf bar (siehe WindowClassCreate Parameter)
/// anders wären sie nicht von der WINAPI verwendbar.

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pStruktur = reinterpret_cast<CREATESTRUCTW*>(lParam);			//lparam des WM_NCCREATE ist eine CREATESTRUCTW -> umwandlung in CREATESTRUCT* pointer
		Window* const pWindow = static_cast<Window*>(pStruktur->lpCreateParams);			//speichern der CreateParam aus CREATESTRUCTW als Window Pointer
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));				//setzt GWLP_USERDATA zu dem Pointer zu dem Fenster
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgAPICon));		//setzt GWLP_WNDPROC zu dem Pointer der HandleMsgSetup2 Funktion
		return pWindow->HandleMsg(hWnd, msg, wParam, lParam, *pWindow);										//für weitere Verarbeitung von messages aufruf der HandleMsg funktion
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgAPICon(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));				//der pointer zu dem Fenster wird extrahiert aus der WinAPI
	return pWindow->HandleMsg(hWnd, msg, wParam, lParam, *pWindow);											//aus diesem Pointer wird die HandleMsg Funktion als pointer returnt (und ausgeführt)
}

#pragma endregion