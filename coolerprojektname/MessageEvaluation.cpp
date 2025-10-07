#include <sstream>
#include <iostream>
#include <array>
#include <bitset>
#include "Window.h"
#include "DebugTools.h"
#include "Camera.h"


LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Window& pWindow) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		delete &pWindow;
		if (WindowCount == 0)
		{
			PostQuitMessage(69);
		}
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		//Position
		case 'W':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | DirectionZn;
			break;
		}
		case 'A':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | DirectionX;
			break;
		}
		case 'S':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | (DirectionZ);
			break;
		}
		case 'D':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | DirectionXn;
			break;
		}
		case VK_SPACE:
		{
			pWindow.HeldKeys = pWindow.HeldKeys | DirectionYn;
			break;
		}
		case VK_CONTROL:
		{
			if (!(lParam & (1 << 24))) //Die gedrückte Controll Taste, ist die Linke -> angegeben im 24. Bit des lParam
			{
				pWindow.HeldKeys = pWindow.HeldKeys | DirectionY;
				break;
			}
		}
		//Rotation
		case VK_UP:
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationPitchn;
			break;
		}
		case VK_DOWN:
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationPitch;
			break;
		}
		case VK_LEFT:
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationYawn;
			break;
		}
		case VK_RIGHT:
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationYaw;
			break;
		}
		case 'Q':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationRoll;
			break;
		}
		case 'E':
		{
			pWindow.HeldKeys = pWindow.HeldKeys | RotationRolln;
			break;
		}
		}
		break;
	}
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case 'W':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionZn;
			break;
		}
		case 'A':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionX;
			break;
		}
		case 'S':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionZ;
			break;
		}
		case 'D':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionXn;
			break;
		}
		case VK_SPACE:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionYn;
			break;
		}
		case VK_CONTROL:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - DirectionY;
			break;
		}

		//Rotation
		case VK_UP:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationPitchn;
			break;
		}
		case VK_DOWN:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationPitch;
			break;
		}
		case VK_LEFT:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationYawn;
			break;
		}
		case VK_RIGHT:
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationYaw;
			break;
		}
		case 'Q':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationRoll;
			break;
		}
		case 'E':
		{
			pWindow.HeldKeys = pWindow.HeldKeys - RotationRolln;
			break;
		}
		}
		break;
	}
	case WM_SIZE:
	{
		for (size_t i = 0; i < Window::WindowCount; i++)
		{
			if (Window::WindowList[i]->hWindow == hWnd)
			{
				Window* foundwindow = Window::WindowList[i];
				RECT wr;
				GetWindowRect(hWnd, &wr);
				foundwindow->Width = wr.right - wr.left;
				foundwindow->Height = wr.bottom - wr.top;
				foundwindow->pGraphicsGet().UpdateAspectratio(foundwindow->Width,foundwindow->Height);
			}
		}
		break;
	}
	}
	 
	std::cout << Outputlog::msgIdConv(msg) << " " << " " << (char)wParam << " " << std::bitset<64>(lParam) << std::endl;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}