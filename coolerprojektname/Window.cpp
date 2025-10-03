#include "Window.h"
#include <string>
#include <sstream>
#include "resource.h"
#include "Camera.h"

//Definition des statischen Window::WindowClass Objekts
Window::WindowClass Window::WindowClass::wndClass;

//Definition der Statischen Variablen
unsigned long Window::WindowCount = 0;
std::vector<Window*> Window::windowlist = {};

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

Window::Window(unsigned short width, unsigned short height, const char* name, const graphicsstruct& sGfx) noexcept
	:
	height(height),
	width(width),
	title(name),
	sGfx(&sGfx)
{
	RECT wr;
	int maximize = NULL; //Maximieren des Fensters, wenn Fensterdimension außerhalb der gültigen Dimensionen ist

	//Erstellen eines Rechtecks, der Größe des Fensters
	wr.left = 0;
	wr.top = 0;
	if	((height == 0 && width == 0) || 
		(height > GetSystemMetrics(SM_CYFULLSCREEN) && width > GetSystemMetrics(SM_CXFULLSCREEN))) {
		Window::height = GetSystemMetrics(SM_CYFULLSCREEN)-100;
		Window::width = GetSystemMetrics(SM_CXFULLSCREEN)-500;
		maximize = WS_MAXIMIZE;
	}
	wr.right = Window::width + wr.left;
	wr.bottom = Window::height + wr.top;

	//Adjustieren des Rechtecks an die vorhandene Menüleiste
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX| WS_SYSMENU, FALSE);

	//Fenster erstellen
	hWnd = CreateWindowExA(0,
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE | maximize,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, WindowClass::GetInstance(), this
	);

	if (maximize) {
		GetWindowRect(hWnd, &wr);
		Window::height = wr.bottom - wr.top;
		Window::width = wr.right - wr.left;
	}

	//Fenstertitel korrigieren (keine Ahnung warum der beim Erstellen nicht richtig ist)
	SetWindowTextA(hWnd, (char*)&title[0]);

	//Erstellen des Graphics Objektes
	pGfx = std::make_unique<Graphics>(hWnd);

	//Window Pointer Management
	windowlist.push_back(this);
	WindowCount++;
}

Window::~Window()
{
	auto objname = std::find(windowlist.begin(), windowlist.end(), this);
	windowlist.erase(objname);
	DestroyWindow(hWnd);
	WindowCount--;
}

#pragma endregion

#pragma region Function for getting Graphics Object

Graphics& Window::pGraphics()
{
	return *pGfx;
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
		return pWindow->HandleMsg(hWnd, msg, wParam, lParam);										//für weitere Verarbeitung von messages aufruf der HandleMsg funktion
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgAPICon(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));				//der pointer zu dem Fenster wird extrahiert aus der WinAPI
	return pWindow->HandleMsg(hWnd, msg, wParam, lParam);													//aus diesem Pointer wird die HandleMsg Funktion als pointer returnt (und ausgeführt)
}

#pragma endregion