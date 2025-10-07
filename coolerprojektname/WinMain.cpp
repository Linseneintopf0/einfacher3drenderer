#include "window.h"
#include "Scene.h"
#include "Color.h"
#include "DebugTools.h"
#include <iostream>
#include <vector>
#include <memory>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
		//Erstellen von Grafik Strukturen:
		Window::GraphicsStructure tg1;
			tg1.FilePath = "teapot.obj"; //!!Ausschlie�lich ASCII/UTF-8 Zeichen!!
			tg1.ColorObj = std::make_unique<Color>(255, 255, 255, 1); //Hintergrundfarbe
			tg1.scale = 3; //Korrekturlevel f�r Risse im Model


			if (
				false					   //Message Pump in LogWindow? true/false
				){Outputlog::AttachConsoleWindow();}

		Window* wnd1 = new Window(
			0,							//Breite des Fensters
			0,							//H�he des Fensters
			"Window one",				//Name des Fensters
			tg1);						//Name einer Grafikstruktur

		//DirectX ged�ns Erstellen
		
		MSG Msg;
		Msg.message = NULL;

		MSG PrevMsg;
		PrevMsg = {0};

		BOOL bPeekMessageResult;
		while (Msg.message != WM_QUIT)
		{
			bPeekMessageResult = PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&Msg);
			if (Msg.message == WM_KEYDOWN || Msg.message == WM_KEYUP) //Alle Nicht-Keyboard-Messages passen lassen
			{
				//filtern f�r widerhohlende Nachichten, au�er bei welchen f�r Verschiedene Tasten UND Filtern f�r Tastendr�cke, die wiederhohlend sind
				if ( ((PrevMsg.message != Msg.message) || (PrevMsg.wParam != Msg.wParam)) && (Msg.message != WM_KEYDOWN || !(Msg.lParam & ((long long)1 << 31))) )
				{ 
					DispatchMessageA(&Msg); 
				}
			} 
			else 
			{
				DispatchMessageA(&Msg);
			}
			for (unsigned int i = 0; i < Window::WindowCount; i++)
			{
				Scene::DoFrame(*Window::WindowList[i]);
			}
			PrevMsg = Msg;
		}
		if (Msg.message == WM_QUIT)
		{
			return (int)Msg.wParam;
		}
		else
		{
			return 0;
		}
}