#include "window.h"
#include "wcharconv.h"
#include "gfxupdate.h"
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

	try {

		//Erstellen von Farben
		Color white(1.0f, 1.0f, 1.0f, 1);
		Color black(0.0f, 0.0f, 0.0f, 1);
		Color aquablue(0.0f, 0.5f, 1.0f, 1);


		//Erstellen von Grafik Strukturen:
		Window::graphicsstruct bg1;
			bg1.dateipfad = "";				//DateiPfad (Wenn leer nur Hintergrund) oder KEYWORD (z.B. RAINBOW)
			bg1.colorobj =& black;			//Farbe für den Hintergrund (muss als Pointer weiter gegeben werden)

		Window::graphicsstruct bg2;
			bg2.dateipfad = "RAINBOW";
			bg2.colorobj =& aquablue;

		Window::graphicsstruct tg1;
			tg1.dateipfad = "TRIANGLE";
			tg1.colorobj =& aquablue;


			if (
				false					   //Message Pump in LogWindow? true/false
				){Outputlog::AttachConsoleWindow();}


		Window* wnd1 = new Window(
			800,							//Breite des Fensters
			600,							//Höhe des Fensters
			"Window one",					//Name des Fensters
			tg1);							//Name einer Grafikstruktur



		MSG msg;
		msg.message = NULL;
		BOOL bPMResult;
		while (msg.message != WM_QUIT)
		{
			if (msg.message != WM_CHAR)
			{
				bPMResult = PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE);
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
				for (unsigned int i = 0; i < Window::WindowCount; i++)
				{
					gfxupdate::DoFrame(*Window::windowlist[i]);
				}
			}
			else
			{
				GetMessageA(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
				for (unsigned int i = 0; i < Window::WindowCount; i++)
				{
					gfxupdate::DoFrame(*Window::windowlist[i]);
				}
			}
		}
	}
	catch (const Ausnahmen& e)
	{
		MessageBoxA(nullptr,e.what(), e.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Ausnahmefall", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Standard Ausnahmefall", MB_OK | MB_ICONERROR);
	}
}