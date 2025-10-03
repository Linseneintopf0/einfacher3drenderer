#include "window.h"
#include "Scene.h"
#include "Color.h"
#include "DebugTools.h"
#include <iostream>
#include <vector>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

		//Erstellen von Farben
		Color white(1.0f, 1.0f, 1.0f, 1);
		Color black(0.0f, 0.0f, 0.0f, 1);
		Color aquablue(0.0f, 0.5f, 1.0f, 1);


		//Erstellen von Grafik Strukturen:
		Window::graphicsstruct tg1;
			tg1.dateipfad = "CUBE";
			tg1.colorobj =& white;


			if (
				false					   //Message Pump in LogWindow? true/false
				){Outputlog::AttachConsoleWindow();}

		Window* wnd1 = new Window(
			0,							//Breite des Fensters
			0,							//Höhe des Fensters
			"Window one",				//Name des Fensters
			tg1);						//Name einer Grafikstruktur

		//DirectX gedöns Erstellen
		for (unsigned int i = 0; i < Window::WindowCount; i++)
		{
			Scene::SetupFrame(*Window::windowlist[i]);
		}

		MSG msg;
		msg.message = NULL;
		BOOL bPMResult;
		while (msg.message != WM_QUIT)
		{
			bPMResult = PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			for (unsigned int i = 0; i < Window::WindowCount; i++)
			{
				Scene::DoFrame(*Window::windowlist[i]);
			}
		}
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		else
		{
			return 0;
		}
}