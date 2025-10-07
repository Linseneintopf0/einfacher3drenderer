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
			tg1.FilePath = "teapot.obj"; //!!Ausschließlich ASCII/UTF-8 Zeichen!!
			tg1.ColorObj = std::make_unique<Color>(255, 255, 255, 1); //Hintergrundfarbe
			tg1.scale = 3; //Korrekturlevel für Risse im Model


			if (
				false					   //Message Pump in LogWindow? true/false
				){Outputlog::AttachConsoleWindow();}

		Window* wnd1 = new Window(
			0,							//Breite des Fensters
			0,							//Höhe des Fensters
			"Window one",				//Name des Fensters
			tg1);						//Name einer Grafikstruktur

		//DirectX gedöns Erstellen
		
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
				//filtern für widerhohlende Nachichten, außer bei welchen für Verschiedene Tasten UND Filtern für Tastendrücke, die wiederhohlend sind
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